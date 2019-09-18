#include "camera.hpp"

#ifndef _CONFIG_PLATFORM_WINDOWS
#include <stdio.h>

#include <asm/types.h>
#include <linux/videodev2.h>

#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
// TODO: remove. used for memset.
#include <memory.h>

#include <iostream>

namespace sailbot { namespace camera {
    static int device_file_id;
    static constexpr unsigned int buffer_count = 4;
    static v4l2_buffer bufferinfo[buffer_count];
    static void *buffer_start[buffer_count];
    static int buffer_index = 0;
    bool open(const char *const device_filepath) {
        device_file_id = ::open(device_filepath, O_RDWR);
        if (!device_file_id) {
            std::cout << "unable to open video source\n";
            return false;
        }
        // GET CAPABILITIES
        v4l2_capability capability;
        if (ioctl(device_file_id, VIDIOC_QUERYCAP, &capability)) {
            std::cout << "unable to query video capability\n";
            ::close(device_file_id);
            return false;
        }
        if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
            std::cout << "device does not handle single-planar video capture.\n";
            ::close(device_file_id);
            return false;
        }
        // SET FORMAT
        v4l2_format format;
        format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        format.fmt.pix.width = width;
        format.fmt.pix.height = height;
        if (ioctl(device_file_id, VIDIOC_S_FMT, &format) < 0) {
            std::cout << "unable to set video capture format\n";
            ::close(device_file_id);
            return false;
        }
        // SET FRAMERATE
        v4l2_streamparm streamparam;
        streamparam.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        streamparam.parm.capture.timeperframe.numerator = 1;
        streamparam.parm.capture.timeperframe.denominator = 30;
        if (ioctl(device_file_id, VIDIOC_S_PARM, &streamparam) == -1 ||
            ioctl(device_file_id, VIDIOC_G_PARM, &streamparam) == -1) {
            printf("unable to set framerate\n");
            return false;
        }
        // PREPARE FOR BUFFER HANDLING
        v4l2_requestbuffers bufrequest;
        bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        bufrequest.memory = V4L2_MEMORY_MMAP;
        bufrequest.count = 4;
        if (ioctl(device_file_id, VIDIOC_REQBUFS, &bufrequest) < 0) {
            std::cout << "unable to request video buffers\n";
            ::close(device_file_id);
            return false;
        }
        for (unsigned char i = 0; i < buffer_count; ++i) {
            bufferinfo[i].type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            bufferinfo[i].memory = V4L2_MEMORY_MMAP;
            bufferinfo[i].index = i;
            if (ioctl(device_file_id, VIDIOC_QUERYBUF, &bufferinfo[i]) < 0) {
                std::cout << "unable to query video buffers\n";
                ::close(device_file_id);
                return false;
            }
            // MAP MEMORY
            buffer_start[i] =
                mmap(NULL, bufferinfo[i].length, PROT_READ | PROT_WRITE, MAP_SHARED, device_file_id, bufferinfo[i].m.offset);
            if (buffer_start[i] == MAP_FAILED) {
                perror("mmap");
                return false;
            }
            memset(buffer_start[i], 0, bufferinfo[i].length);
        }
        // PREPARE STREAMING INFO
        // Activate streaming
        int type = bufferinfo[0].type;
        if (ioctl(device_file_id, VIDIOC_STREAMON, &type) < 0) {
            perror("VIDIOC_STREAMON");
            return false;
        }

        // Put each buffer into the queue
        for (unsigned char i = 1; i < buffer_count; ++i) {
            if (ioctl(device_file_id, VIDIOC_QBUF, &bufferinfo[i]) < 0) {
                perror("VIDIOC_QBUF");
                return false;
            }
        }
        return true;
    }
    unsigned char *read() {
        if (ioctl(device_file_id, VIDIOC_QBUF, &bufferinfo[buffer_index]) < 0) {
            perror("VIDIOC_QBUF2");
            exit(1);
        }

        ++buffer_index;
        if (buffer_index == buffer_count) buffer_index = 0;

        // The buffer's waiting in the outgoing queue.
        if (ioctl(device_file_id, VIDIOC_DQBUF, &bufferinfo[buffer_index]) < 0) {
            perror("VIDIOC_DQBUF");
            exit(1);
        }

        return reinterpret_cast<unsigned char *>(buffer_start[buffer_index]);
    }
    void close() {
        // Deactivate streaming
        int type = bufferinfo[0].type;
        if (ioctl(device_file_id, VIDIOC_STREAMOFF, &type) < 0) {
            perror("VIDIOC_STREAMOFF");
            exit(1);
        }
        ::close(device_file_id);
    }
}} // namespace sailbot::camera
#else
#include <stdio.h>
namespace sailbot { namespace camera {
    static unsigned char data[2 * width * height];
    bool open(const char *const device_filepath) {
        printf("[WARNING] The camera API is not supported on Windows!\n"
               "All data recieved from `sailbot::camera::read()` will "
               "be set to 0.");
        for (unsigned int i = 0; i < 2 * width * height; ++i)
            data[i] = 0;
    }
    unsigned char *read() { return data; }
    void close() {}
}} // namespace sailbot::camera
#endif // !_CONFIG_PLATFORM_WINDOWS
