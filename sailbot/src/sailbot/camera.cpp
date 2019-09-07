#include "camera.hpp"
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
    static v4l2_buffer bufferinfo;
    static void *buffer_start;
    void open() {
        device_file_id = ::open("/dev/video0", O_RDWR);
        if (!device_file_id) { std::cout << "unable to open video source"; }
        // GET CAPABILITIES
        v4l2_capability capability;
        if (ioctl(device_file_id, VIDIOC_QUERYCAP, &capability)) {
            std::cout << "unable to query video capability";
            ::close(device_file_id);
        }
        if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
            std::cout << "device does not handle single-planar video capture.\n";
            ::close(device_file_id);
        }
        // SET FORMAT
        v4l2_format format;
        format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        format.fmt.pix.width = width;
        format.fmt.pix.height = height;
        if (ioctl(device_file_id, VIDIOC_S_FMT, &format) < 0) {
            std::cout << "unable to set video capture format";
            ::close(device_file_id);
        }
        // SET FRAMERATE
        v4l2_streamparm streamparam;
        streamparam.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        streamparam.parm.capture.timeperframe.numerator = 1;
        streamparam.parm.capture.timeperframe.denominator = 30;
        if (ioctl(device_file_id, VIDIOC_S_PARM, &streamparam) == -1 ||
            ioctl(device_file_id, VIDIOC_G_PARM, &streamparam) == -1) {
            printf("unable to set framerate\n");
        }
        // PREPARE FOR BUFFER HANDLING
        v4l2_requestbuffers bufrequest;
        bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        bufrequest.memory = V4L2_MEMORY_MMAP;
        bufrequest.count = 1;
        if (ioctl(device_file_id, VIDIOC_REQBUFS, &bufrequest) < 0) {
            std::cout << "unable to request video buffers";
            ::close(device_file_id);
        }
        memset(&bufferinfo, 0, sizeof(bufferinfo));
        bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        bufferinfo.memory = V4L2_MEMORY_MMAP;
        bufferinfo.index = 0;
        if (ioctl(device_file_id, VIDIOC_QUERYBUF, &bufferinfo) < 0) {
            std::cout << "unable to query video buffers";
            ::close(device_file_id);
        }
        // MAP MEMORY
        buffer_start = mmap(NULL, bufferinfo.length, PROT_READ | PROT_WRITE, MAP_SHARED, device_file_id, bufferinfo.m.offset);
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
    }
    unsigned char *read() {
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
        return reinterpret_cast<unsigned char *>(buffer_start);
    }
    void close() {
        // Deactivate streaming
        int type = bufferinfo.type;
        if (ioctl(device_file_id, VIDIOC_STREAMOFF, &type) < 0) {
            perror("VIDIOC_STREAMOFF");
            exit(1);
        }
        ::close(device_file_id);
    }
}} // namespace sailbot::camera
