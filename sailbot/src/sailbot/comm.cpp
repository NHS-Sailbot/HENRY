#ifndef _CONFIG_PLATFORM_WINDOWS
#include "comm.hpp"
#include "timing.hpp"

#include <cstdio>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace sailbot { namespace arduino {
    enum OpenDeviceError : const unsigned char {
        SUCCESS = 0,
        INVALID_BAUDRATE = 1,
        UNABLE_TO_OPEN_PORT = 2,
        SPECIFIED_PORT_IS_LOCKED = 3,
        UNABLE_TO_GET_PORT_SETTINGS = 4,
        UNABLE_TO_SET_PORT_SETTINGS = 5,
        UNABLE_TO_GET_PORT_STATUS = 6,
        UNABLE_TO_SET_PORT_STATUS = 7
    };

    static int s_device_file_id;
    static termios port_settings, old_port_settings;

    bool connect(const char *const filepath, unsigned int baudrate) {
        switch (baudrate) {
        case 50: baudrate = B50; break;
        case 75: baudrate = B75; break;
        case 110: baudrate = B110; break;
        case 134: baudrate = B134; break;
        case 150: baudrate = B150; break;
        case 200: baudrate = B200; break;
        case 300: baudrate = B300; break;
        case 600: baudrate = B600; break;
        case 1200: baudrate = B1200; break;
        case 1800: baudrate = B1800; break;
        case 2400: baudrate = B2400; break;
        case 4800: baudrate = B4800; break;
        case 9600: baudrate = B9600; break;
        case 19200: baudrate = B19200; break;
        case 38400: baudrate = B38400; break;
        case 57600: baudrate = B57600; break;
        case 115200: baudrate = B115200; break;
        case 230400: baudrate = B230400; break;
        case 460800: baudrate = B460800; break;
        case 500000: baudrate = B500000; break;
        case 576000: baudrate = B576000; break;
        case 921600: baudrate = B921600; break;
        case 1000000: baudrate = B1000000; break;
        case 1152000: baudrate = B1152000; break;
        case 1500000: baudrate = B1500000; break;
        case 2000000: baudrate = B2000000; break;
        case 2500000: baudrate = B2500000; break;
        case 3000000: baudrate = B3000000; break;
        case 3500000: baudrate = B3500000; break;
        case 4000000: baudrate = B4000000; break;
        default: return false;
        }
        s_device_file_id = open(filepath, O_RDWR | O_NOCTTY | O_NDELAY);
        if (s_device_file_id == -1) return UNABLE_TO_OPEN_PORT;
        if (flock(s_device_file_id, LOCK_EX | LOCK_NB)) {
            close(s_device_file_id);
            return false;
        }
        int error = tcgetattr(s_device_file_id, &old_port_settings);
        if (error == -1) {
            close(s_device_file_id);
            flock(s_device_file_id, LOCK_UN);
            return false;
        }
        constexpr static const int cbits = CS8, cpar = 0, ipar = IGNPAR, bstop = 0;
        port_settings = {
            ipar, 0, cbits | cpar | bstop | CLOCAL | CREAD,
            0,    0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            0,    0,
        };
        cfsetispeed(&port_settings, baudrate);
        cfsetospeed(&port_settings, baudrate);
        error = tcsetattr(s_device_file_id, TCSANOW, &port_settings);
        if (error == -1) {
            tcsetattr(s_device_file_id, TCSANOW, &old_port_settings);
            close(s_device_file_id);
            flock(s_device_file_id, LOCK_UN);
            return false;
        }
        int status;
        if (ioctl(s_device_file_id, TIOCMGET, &status) == -1) {
            tcsetattr(s_device_file_id, TCSANOW, &old_port_settings);
            flock(s_device_file_id, LOCK_UN);
            return false;
        }
        status |= TIOCM_DTR;
        status |= TIOCM_RTS;
        if (ioctl(s_device_file_id, TIOCMSET, &status) == -1) {
            tcsetattr(s_device_file_id, TCSANOW, &old_port_settings);
            flock(s_device_file_id, LOCK_UN);
            return false;
        }

        timing::sleep(1.f);
        tcflush(s_device_file_id, TCIOFLUSH);

        // TODO: Add ping back check to see if it is THE arduino

        return true;
    }

    void transmit(const void *const data, const unsigned int size) { write(s_device_file_id, data, size); }

    void receive(void *const data, const unsigned int size) { read(s_device_file_id, data, size); }

    void disconnect() { close(s_device_file_id); }
}} // namespace sailbot::arduino
#else
namespace sailbot { namespace arduino {
    bool connect(const char *const filepath, unsigned int baudrate) { return true; }

    void transmit(const void *const data, const unsigned int size) {}

    void receive(void *const data, const unsigned int size) {}

    void disconnect() {}
}} // namespace sailbot::arduino
#endif
