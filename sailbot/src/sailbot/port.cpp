#include <cstdio>

namespace sailbot { namespace comm {
#ifndef _CONFIG_PLATFORM_WINDOWS
#include <sys/file.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

    constexpr static const unsigned int PORT_COUNT = 38;
    constexpr static const char *DEVICE_FILE[PORT_COUNT] = {
        "/dev/ttyS0",   "/dev/ttyS1",   "/dev/ttyS2",   "/dev/ttyS3",   "/dev/ttyS4",   "/dev/ttyS5",   "/dev/ttyS6",
        "/dev/ttyS7",   "/dev/ttyS8",   "/dev/ttyS9",   "/dev/ttyS10",  "/dev/ttyS11",  "/dev/ttyS12",  "/dev/ttyS13",
        "/dev/ttyS14",  "/dev/ttyS15",  "/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4",
        "/dev/ttyUSB5", "/dev/ttyAMA0", "/dev/ttyAMA1", "/dev/ttyACM0", "/dev/ttyACM1", "/dev/rfcomm0", "/dev/rfcomm1",
        "/dev/ircomm0", "/dev/ircomm1", "/dev/cuau0",   "/dev/cuau1",   "/dev/cuau2",   "/dev/cuau3",   "/dev/cuaU0",
        "/dev/cuaU1",   "/dev/cuaU2",   "/dev/cuaU3"};
    constexpr static const unsigned int UNABLE_TO_OPEN_PORT = 1, SPECIFIED_PORT_IS_LOCKED = 2, UNABLE_TO_GET_PORT_SETTINGS = 3,
                                        UNABLE_TO_SET_PORT_SETTINGS = 4, UNABLE_TO_GET_PORT_STATUS = 5,
                                        UNABLE_TO_SET_PORT_STATUS = 6, SUCCESS = 0;

    static int file_id[PORT_COUNT];
    static termios port_settings, old_port_settings[PORT_COUNT];

    int open_device(const unsigned int port) {
        file_id[port] = open(DEVICE_FILE[port], O_RDWR | O_NOCTTY | O_NDELAY);
        if (file_id[port] == -1)
            return UNABLE_TO_OPEN_PORT;
        if (flock(file_id[port], LOCK_EX | LOCK_NB)) {
            close(file_id[port]);
            return SPECIFIED_PORT_IS_LOCKED;
        }
        int error = tcgetattr(file_id[port], old_port_settings + port);
        if (error == -1) {
            close(file_id[port]);
            flock(file_id[port], LOCK_UN);
            return UNABLE_TO_GET_PORT_SETTINGS;
        }
        constexpr static const int baudrate = B57600, cbits = CS8, cpar = 0, ipar = IGNPAR, bstop = 0;
        port_settings = {
            ipar, 0, cbits | cpar | bstop | CLOCAL | CREAD,
            0,    0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            0,    0,
        };
        cfsetispeed(&port_settings, baudrate);
        cfsetospeed(&port_settings, baudrate);
        error = tcsetattr(file_id[port], TCSANOW, &port_settings);
        if (error == -1) {
            tcsetattr(file_id[port], TCSANOW, old_port_settings + port);
            close(file_id[port]);
            flock(file_id[port], LOCK_UN);
            return UNABLE_TO_SET_PORT_SETTINGS;
        }
        int status;
        if (ioctl(file_id[port], TIOCMGET, &status) == -1) {
            tcsetattr(file_id[port], TCSANOW, old_port_settings + port);
            flock(file_id[port], LOCK_UN);
            return UNABLE_TO_GET_PORT_STATUS;
        }
        status |= TIOCM_DTR;
        status |= TIOCM_RTS;
        if (ioctl(file_id[port], TIOCMSET, &status) == -1) {
            tcsetattr(file_id[port], TCSANOW, old_port_settings + port);
            flock(file_id[port], LOCK_UN);
            return UNABLE_TO_SET_PORT_STATUS;
        }
        return 0;
    }

    void write_buffer(const unsigned int port, const void *data, const unsigned int size) { write(file_id[port], data, size); }
    void read_device(const unsigned int port, void *data, const unsigned int size) { read(file_id[port], data, size); }

#else
    void write_buffer(const unsigned int port, const void *data, const unsigned int size) {}
    void read_device(const unsigned int port, void *data, const unsigned int size) {}
#endif
    int open_device_err(const unsigned int port) {
        switch (open_device(port)) {
        case UNABLE_TO_OPEN_PORT:
            printf("unable to open port %d (%s)\n", port, DEVICE_FILE[port]);
            return 1;
        case SPECIFIED_PORT_IS_LOCKED:
            printf("port %d (%s) is locked\n", port, DEVICE_FILE[port]);
            return 1;
        case UNABLE_TO_GET_PORT_SETTINGS:
            printf("unable to get port %d's (%s) settings\n", port, DEVICE_FILE[port]);
            return 1;
        case UNABLE_TO_SET_PORT_SETTINGS:
            printf("unable to set port %d's (%s) settings\n", port, DEVICE_FILE[port]);
            return 1;
        case UNABLE_TO_GET_PORT_STATUS:
            printf("unable to get port %d's (%s) status\n", port, DEVICE_FILE[port]);
            return 1;
        case UNABLE_TO_SET_PORT_STATUS:
            printf("unable to set port %d's (%s) status\n", port, DEVICE_FILE[port]);
            return 1;
        case SUCCESS:
            printf("SUCCESS! opened port %d (%s)\n", port, DEVICE_FILE[port]);
            return 0;
        default:
            printf("unhandled case; port %d (%s)\n", port, DEVICE_FILE[port]);
            return 1;
        }
    }
}} // namespace sailbot::comm
