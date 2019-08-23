#include <glad/glad.h>
//
#include <GLFW/glfw3.h>

int main() {
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(1000, 1000, "test", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_QUADS);
        glVertex2f(-0.9f, -0.9f);
        glVertex2f(-0.9f, 0.9f);
        glVertex2f(0.9f, 0.9f);
        glVertex2f(0.9f, -0.9f);
        glEnd();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

/*
    #include <memory.h>
    #include <sys/file.h>
    #include <sys/ioctl.h>
    #include <sys/mman.h>
    #include <unistd.h>

    #include <asm/types.h>
    #include <linux/videodev2.h>

    #include <iostream>

    inline constexpr unsigned char clamp(const int val) {
        if (val > 255) return 255;
        if (val < 0) return 0;
        return val;
    }

    static constexpr unsigned int capture_width = 1920, capture_height = 1080;

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
        {
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
            void *buffer_start =
                mmap(NULL, bufferinfo.length, PROT_READ | PROT_WRITE, MAP_SHARED, device_file_id, bufferinfo.m.offset);
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


            for (unsigned int i = 0; i < 1000000; ++i) {

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

                struct ColorRGB {
                    unsigned char r, g, b;
                };

                ColorRGB image_data[capture_width * capture_height];
                unsigned char *cam_y = reinterpret_cast<unsigned char *>(buffer_start),
                            *cam_u = reinterpret_cast<unsigned char *>(buffer_start) + 1,
                            *cam_v = reinterpret_cast<unsigned char *>(buffer_start) + 3;

                for (unsigned int y = 0; y < capture_height; ++y) {
                    for (unsigned int x = 0; x < capture_width; ++x) {
                        const unsigned int index = x + y * capture_width;
                        ColorRGB &image_p = image_data[index];
                        const int c = *cam_y - 16, d = *cam_u - 128, e = *cam_v - 128;
                        image_p.r = clamp((298 * c + 409 * e + 128) >> 8);
                        image_p.g = clamp((298 * c - 100 * d - 208 * e + 128) >> 8);
                        image_p.b = clamp((298 * c + 516 * d + 128) >> 8);
                        if (index % 2) { cam_u += 4, cam_v += 4; }
                        cam_y += 2;
                    }
                }

                {
                    // CREATE IMAGE
                    FILE *img_file_id;
                    if ((img_file_id = fopen("/home/gabe/Downloads/myimage.ppm", "wb")) < 0) {
                        perror("open");
                        exit(1);
                    }
                    fprintf(img_file_id, "P6\n%d %d\n255\n", capture_width, capture_height);
                    fwrite(image_data, 1, sizeof(image_data), img_file_id);
                    fclose(img_file_id);
                }

                // usleep(250000);
            }

            // Deactivate streaming
            if (ioctl(device_file_id, VIDIOC_STREAMOFF, &type) < 0) {
                perror("VIDIOC_STREAMOFF");
                exit(1);
            }
        }

        close(device_file_id);
    }
*/
