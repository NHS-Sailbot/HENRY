#include <sailbot.hpp>

#include <iostream>
#include <memory.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <asm/types.h>
#include <linux/videodev2.h>

#include <linux/v4l2-subdev.h>

static constexpr unsigned int capture_width = 1600, capture_height = 900;
struct Vertex {
    float x, y, u, v;
};

static constexpr Vertex vertices[]{
    {-1, -1, 0, 1},
    {-1, 1, 0, 0},
    {1, -1, 1, 1},
    {1, 1, 1, 0},
};

static constexpr unsigned short indices[] = {0, 1, 2, 1, 3, 2};

static constexpr char *vert_src = R"(
    #version 330 core
    layout(location = 0) in vec2 pos;
    layout(location = 1) in vec2 tex;
    layout(location = 2) in vec4 col;
    layout(location = 3) in float tid;
    
    out vec2 v_tex;
    
    void main() {
        v_tex = tex;
        gl_Position = vec4(pos, 0, 1);
    }
)";

static constexpr char *frag_src = R"(
    #version 330 core
    in vec2 v_tex;
    
    out vec4 color;
    
    uniform sampler2D tex;
    
    vec4 yuv422_rgb(in vec4 col) {
        col -= vec4(0.0625, 0.5, 0.5, 0);
        vec4 result = vec4(
            1.1640625 * col.x + 1.59765625 * col.z + 0.5,
            0,0,1
        );
        return result;
    }
    
    void main() {
        color = vec4(v_tex, 0, 1);
        color = yuv422_rgb(texture(tex, v_tex));
    }
)";

// const int c = *cam_y - 16, d = *cam_u - 128, e = *cam_v - 128;
// image_r = clamp((298 * c + 409 * e + 128) >> 8);
// image_g = clamp((298 * c - 100 * d - 208 * e + 128) >> 8);
// image_b = clamp((298 * c + 516 * d + 128) >> 8);

int main() {
    // OPEN DEVICE
    int device_file_id = open("/dev/video0", O_RDWR);
    if (!device_file_id) {
        std::cout << "unable to open video source";
        return -1;
    }
    {
        // GET CAPABILITIES
        v4l2_capability capability;
        if (ioctl(device_file_id, VIDIOC_QUERYCAP, &capability)) {
            std::cout << "unable to query video capability";
            close(device_file_id);

            return -1;
        }
        if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
            std::cout << "device does not handle single-planar video capture.\n";
            close(device_file_id);
            return -1;
        }
    }
    {
        // SET FORMAT
        v4l2_format format;
        format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        format.fmt.pix.width = capture_width;
        format.fmt.pix.height = capture_height;
        if (ioctl(device_file_id, VIDIOC_S_FMT, &format) < 0) {
            std::cout << "unable to set video capture format";
            close(device_file_id);
            return -1;
        }
    }
    {
        // SET FRAMERATE
        v4l2_streamparm streamparam;
        streamparam.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        streamparam.parm.capture.timeperframe.numerator = 1;
        streamparam.parm.capture.timeperframe.denominator = 30;
        if (ioctl(device_file_id, VIDIOC_S_PARM, &streamparam) == -1 ||
            ioctl(device_file_id, VIDIOC_G_PARM, &streamparam) == -1) {
            printf("unable to set framerate\n");
            return -1;
        }
    }
    {
        // PREPARE FOR BUFFER HANDLING
        v4l2_requestbuffers bufrequest;
        bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        bufrequest.memory = V4L2_MEMORY_MMAP;
        bufrequest.count = 1;
        if (ioctl(device_file_id, VIDIOC_REQBUFS, &bufrequest) < 0) {
            std::cout << "unable to request video buffers";
            close(device_file_id);
            return -1;
        }
    }

    //
    //
    //
    //
    //

    v4l2_buffer bufferinfo;
    memset(&bufferinfo, 0, sizeof(bufferinfo));
    bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferinfo.memory = V4L2_MEMORY_MMAP;
    bufferinfo.index = 0;
    if (ioctl(device_file_id, VIDIOC_QUERYBUF, &bufferinfo) < 0) {
        std::cout << "unable to query video buffers";
        close(device_file_id);
        return -1;
    }
    // MAP MEMORY
    void *buffer_start = mmap(NULL, bufferinfo.length, PROT_READ | PROT_WRITE, MAP_SHARED, device_file_id, bufferinfo.m.offset);
    if (buffer_start == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    memset(buffer_start, 0, bufferinfo.length);
    // PREPARE STREAMING INFO
    // Activate streaming
    int type = bufferinfo.type;
    if (ioctl(device_file_id, VIDIOC_STREAMON, &type) < 0) {
        perror("VIDIOC_STREAMON");
        exit(1);
    }

    //
    //
    //
    //
    //

    coel::Window window(capture_width, capture_height, "test");
    coel::renderer::batch2d::init();
    coel::Shader shader(vert_src, frag_src);
    coel::Texture texture(capture_width, capture_height, coel::ColorSpace::RGB);

    double last_time = window.get_time();
    while (!window.should_close()) {
        const double time = window.get_time();
        const double elapsed = time - last_time;
        last_time = time;
        std::cout << "elapsed time is " << elapsed << '\n';
        // Put the buffer in the incoming queue.
        if (ioctl(device_file_id, VIDIOC_QBUF, &bufferinfo) < 0) {
            perror("VIDIOC_QBUF");
            exit(1);
        }
        // The buffer's waiting in the outgoing queue.
        if (ioctl(device_file_id, VIDIOC_DQBUF, &bufferinfo) < 0) {
            perror("VIDIOC_QBUF");
            exit(1);
        }
        unsigned char image_data[capture_width * capture_height * 3];
        unsigned char *cam_y = reinterpret_cast<unsigned char *>(buffer_start),
                      *cam_u = reinterpret_cast<unsigned char *>(buffer_start) + 1,
                      *cam_v = reinterpret_cast<unsigned char *>(buffer_start) + 3;
        for (unsigned int y = 0; y < capture_height; ++y) {
            for (unsigned int x = 0; x < capture_width; ++x) {
                const unsigned int index = x * 3 + y * capture_width * 3;
                image_data[index] = *cam_y;
                image_data[index + 1] = *cam_u;
                image_data[index + 2] = *cam_v;
                if (index % 2) { cam_u += 4, cam_v += 4; }
                cam_y += 2;
                // const unsigned int index = x * 3 + y * capture_width * 3;
                // unsigned char &image_r = image_data[index];
                // unsigned char &image_g = image_data[index + 1];
                // unsigned char &image_b = image_data[index + 2];
                // const int c = *cam_y - 16, d = *cam_u - 128, e = *cam_v - 128;
                // image_r = clamp((298 * c + 409 * e + 128) >> 8);
                // image_g = clamp((298 * c - 100 * d - 208 * e + 128) >> 8);
                // image_b = clamp((298 * c + 516 * d + 128) >> 8);
                // if (index % 2) { cam_u += 4, cam_v += 4; }
                // cam_y += 2;
            }
        }

        //
        //
        //
        //
        //

        texture.update(image_data);

        coel::renderer::clear();
        coel::renderer::batch2d::submit_rect(-0.9, 0.9, 1.8, -1.8);
        coel::renderer::batch2d::flush();

        window.update();
    }

    //
    //
    //
    //
    //

    // Deactivate streaming
    if (ioctl(device_file_id, VIDIOC_STREAMOFF, &type) < 0) {
        perror("VIDIOC_STREAMOFF");
        exit(1);
    }
    close(device_file_id);
}
