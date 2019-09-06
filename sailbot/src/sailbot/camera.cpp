#include "camera.hpp"
#include <stdio.h>

#include <asm/types.h>
#include <linux/videodev2.h>

#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

namespace sailbot { namespace camera {
    namespace _internal {
        // static bool first_capture = true;
        // static constexpr unsigned int max_buffers = 4;
        // static unsigned int color_format, framerate = 30;
        // static int device_file_id, buffer_count = 4, buffer_index = -1;
        // static timeval timestamp;
        static int device_file_id;
        static inline bool init_v4l() {
            // GET CAPABILITIES
            v4l2_capability capability;
            if (ioctl(device_file_id, VIDIOC_QUERYCAP, &capability)) {
                printf("unable to query video capability");
                return false;
            }
            if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
                printf("device does not handle single-planar video capture.\n");
                return false;
            }
        }
        static inline bool find_window_info() {
            v4l2_format format;
            format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            if (ioctl(device_file_id, VIDIOC_G_FMT, &format) == -1) {
                fprintf(stderr, "VIDEOIO ERROR: V4L2: Could not obtain specifics of capture window.\n");
                return false;
            }
            return true;
        }
        static inline bool choose_color_format() {
            v4l2_format format;
            unsigned int color_format_order[] = {
                V4L2_PIX_FMT_BGR24,  V4L2_PIX_FMT_RGB24,   V4L2_PIX_FMT_YVU420, V4L2_PIX_FMT_YUV420, V4L2_PIX_FMT_YUV411P,
                V4L2_PIX_FMT_YUYV,   V4L2_PIX_FMT_UYVY,    V4L2_PIX_FMT_NV12,   V4L2_PIX_FMT_NV21,   V4L2_PIX_FMT_SBGGR8,
                V4L2_PIX_FMT_SGBRG8, V4L2_PIX_FMT_SN9C10X, V4L2_PIX_FMT_Y16,    V4L2_PIX_FMT_Y10,    V4L2_PIX_FMT_GREY,
            };
            for (unsigned int i = 0; i < sizeof(color_format_order) / sizeof(*color_format_order); ++i) {
                format = v4l2_format();
                format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                format.fmt.pix.pixelformat = color_format_order[i];
                format.fmt.pix.field = V4L2_FIELD_ANY;
                format.fmt.pix.width = width;
                format.fmt.pix.height = height;
                if (ioctl(device_file_id, VIDIOC_S_FMT, &format) != -1) return true;
            }
            return false;
        }
        static inline bool set_framerate(const unsigned int value = 30) {
            v4l2_streamparm streamparam;
            streamparam.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            streamparam.parm.capture.timeperframe.numerator = 1;
            streamparam.parm.capture.timeperframe.denominator = value;
            if (ioctl(device_file_id, VIDIOC_S_PARM, &streamparam) == -1 ||
                ioctl(device_file_id, VIDIOC_G_PARM, &streamparam) == -1) {
                printf("unable to set framerate\n");
                return false;
            }
            return true;
        }
        static inline bool request_buffers() {}
        static inline bool create_buffers() {}
        static inline void release_buffers() {}
        static inline bool init() {
            if (!init_v4l()) return false;
            if (!find_window_info()) return false;
            if (!choose_color_format()) return false;
            if (!set_framerate()) return false;
            if (!request_buffers()) return false;
            if (!create_buffers()) {
                release_buffers();
                return false;
            }
            return true;
        }
        static inline bool read_frame() {
            // v4l2_buffer buf;
            // buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            // buf.memory = V4L2_MEMORY_MMAP;

            // while (ioctl(device_file_id, VIDIOC_DQBUF, &buf) == -1) {
            //     if (!(buf.flags & (V4L2_BUF_FLAG_QUEUED | V4L2_BUF_FLAG_DONE))) {
            //         // Maybe buffer not in the queue? Try to put there
            //         if (ioctl(device_file_id, VIDIOC_QBUF, &buf) == -1) return false;
            //         continue;
            //     }
            //     /* display the error and stop processing */
            //     perror("VIDIOC_DQBUF");
            //     return false;
            // }

            // // We shouldn't use this buffer in the queue while not retrieve frame from it.
            // buffers[buf.index].buffer = buf;
            // buffer_index = buf.index;

            // // set timestamp in capture struct to be timestamp of most recent frame
            // timestamp = buf.timestamp;
            // return true;
        }
        static inline bool grab() {
            // if (first_capture) {
            //     buffer_index = -1;
            //     for (unsigned char i = 0; i < buffer_count; ++i) {
            //         v4l2_buffer buffer;
            //         buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            //         buffer.memory = V4L2_MEMORY_MMAP;
            //         buffer.index = i;
            //         if (ioctl(device_file_id, VIDIOC_QBUF, &buffer) == -1) {
            //             perror("VIDIOC_QBUF 1");
            //             return false;
            //         }
            //     }
            //     v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            //     if (ioctl(device_file_id, VIDIOC_STREAMON, &type) == -1) {
            //         perror("VIDIOC_STREAMON");
            //         return false;
            //     }
            //     first_capture = false;
            // }

            // if (buffer_index >= 0) {
            //     if (ioctl(device_file_id, VIDIOC_QBUF, &buffers[buffer_index].buffer) == -1) {
            //         perror("VIDIOC_QBUF 2");
            //         return false;
            //     }
            // }

            // return read_frame();
        }
        static inline unsigned char *const retreive() {
            // if (buffer_index < 0) return nullptr;
            // return reinterpret_cast<unsigned char *const>(buffers[buffer_index].start);
        }
    } // namespace _internal
    bool open() {
        char *const file_name = "/dev/video0";
        while (file_name[10] < 56) {
            printf("Attempting to open '%s'...\n", file_name);
            _internal::device_file_id = ::open(file_name, O_RDWR | O_NONBLOCK, 0);
            if (_internal::device_file_id == -1) {
                close(_internal::device_file_id);
                ++file_name[10];
            } else {
                printf("Successfully opened '%s'\n", file_name);
                return _internal::init();
            }
        }
    }
    unsigned char *read() {
        if (_internal::grab()) { return _internal::retreive(); }
        printf("unable to grab frame\n");
        return nullptr;
    }
}} // namespace sailbot::camera
