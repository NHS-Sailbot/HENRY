#include <cstdio>

namespace sailbot { namespace comm {
    constexpr static unsigned int PORT_COUNT = 32;
    constexpr static unsigned int INVALID_BAUDRATE = 1, UNABLE_TO_OPEN_PORT = 2, SPECIFIED_PORT_IS_LOCKED = 3,
                                  UNABLE_TO_GET_PORT_SETTINGS = 4, UNABLE_TO_SET_PORT_SETTINGS = 5,
                                  UNABLE_TO_GET_PORT_STATUS = 6, UNABLE_TO_SET_PORT_STATUS = 7, SUCCESS = 0;
#ifndef _CONFIG_PLATFORM_WINDOWS
#include <sys/file.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

    constexpr static const char *DEVICE_FILE[PORT_COUNT] = {
        "/dev/ttyS0",   "/dev/ttyS1",   "/dev/ttyS2",   "/dev/ttyS3",   "/dev/ttyS4",   "/dev/ttyS5",   "/dev/ttyS6",
        "/dev/ttyS7",   "/dev/ttyS8",   "/dev/ttyS9",   "/dev/ttyS10",  "/dev/ttyS11",  "/dev/ttyS12",  "/dev/ttyS13",
        "/dev/ttyS14",  "/dev/ttyS15",  "/dev/ttyAMA0", "/dev/ttyAMA1", "/dev/ttyACM0", "/dev/ttyACM1", "/dev/rfcomm0",
        "/dev/rfcomm1", "/dev/ircomm0", "/dev/ircomm1", "/dev/cuau0",   "/dev/cuau1",   "/dev/cuau2",   "/dev/cuau3",
        "/dev/cuaU0",   "/dev/cuaU1",   "/dev/cuaU2",   "/dev/cuaU3"};

    static int file_id[PORT_COUNT];
    static termios port_settings, old_port_settings[PORT_COUNT];

    int open_device(const unsigned int port, const unsigned int baudrate) {
        unsigned int baud;
        switch (baudrate) {
        case 50:
            baud = B50;
            break;
        case 75:
            baud = B75;
            break;
        case 110:
            baud = B110;
            break;
        case 134:
            baud = B134;
            break;
        case 150:
            baud = B150;
            break;
        case 200:
            baud = B200;
            break;
        case 300:
            baud = B300;
            break;
        case 600:
            baud = B600;
            break;
        case 1200:
            baud = B1200;
            break;
        case 1800:
            baud = B1800;
            break;
        case 2400:
            baud = B2400;
            break;
        case 4800:
            baud = B4800;
            break;
        case 9600:
            baud = B9600;
            break;
        case 19200:
            baud = B19200;
            break;
        case 38400:
            baud = B38400;
            break;
        case 57600:
            baud = B57600;
            break;
        case 115200:
            baud = B115200;
            break;
        case 230400:
            baud = B230400;
            break;
        case 460800:
            baud = B460800;
            break;
        case 500000:
            baud = B500000;
            break;
        case 576000:
            baud = B576000;
            break;
        case 921600:
            baud = B921600;
            break;
        case 1000000:
            baud = B1000000;
            break;
        case 1152000:
            baud = B1152000;
            break;
        case 1500000:
            baud = B1500000;
            break;
        case 2000000:
            baud = B2000000;
            break;
        case 2500000:
            baud = B2500000;
            break;
        case 3000000:
            baud = B3000000;
            break;
        case 3500000:
            baud = B3500000;
            break;
        case 4000000:
            baud = B4000000;
            break;
        default:
            return INVALID_BAUDRATE;
            break;
        }
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
        constexpr static const int cbits = CS8, cpar = 0, ipar = IGNPAR, bstop = 0;
        port_settings = {
            ipar, 0, cbits | cpar | bstop | CLOCAL | CREAD,
            0,    0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            0,    0,
        };
        cfsetispeed(&port_settings, baud);
        cfsetospeed(&port_settings, baud);
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
#include <Windows.h>
    constexpr static const char *DEVICE_FILE[PORT_COUNT] = {
        "\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",  "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",
        "\\\\.\\COM8",  "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12", "\\\\.\\COM13", "\\\\.\\COM14",
        "\\\\.\\COM15", "\\\\.\\COM16", "\\\\.\\COM17", "\\\\.\\COM18", "\\\\.\\COM19", "\\\\.\\COM20", "\\\\.\\COM21",
        "\\\\.\\COM22", "\\\\.\\COM23", "\\\\.\\COM24", "\\\\.\\COM25", "\\\\.\\COM26", "\\\\.\\COM27", "\\\\.\\COM28",
        "\\\\.\\COM29", "\\\\.\\COM30", "\\\\.\\COM31", "\\\\.\\COM32"};
    HANDLE file_id[PORT_COUNT];
    char mode_str[128];
    int open_device(const unsigned int port, const unsigned int baudrate) {
        switch (baudrate) {
        case 110:
            strcpy(mode_str, "baud=110");
            break;
        case 300:
            strcpy(mode_str, "baud=300");
            break;
        case 600:
            strcpy(mode_str, "baud=600");
            break;
        case 1200:
            strcpy(mode_str, "baud=1200");
            break;
        case 2400:
            strcpy(mode_str, "baud=2400");
            break;
        case 4800:
            strcpy(mode_str, "baud=4800");
            break;
        case 9600:
            strcpy(mode_str, "baud=9600");
            break;
        case 19200:
            strcpy(mode_str, "baud=19200");
            break;
        case 38400:
            strcpy(mode_str, "baud=38400");
            break;
        case 57600:
            strcpy(mode_str, "baud=57600");
            break;
        case 115200:
            strcpy(mode_str, "baud=115200");
            break;
        case 128000:
            strcpy(mode_str, "baud=128000");
            break;
        case 256000:
            strcpy(mode_str, "baud=256000");
            break;
        case 500000:
            strcpy(mode_str, "baud=500000");
            break;
        case 921600:
            strcpy(mode_str, "baud=921600");
            break;
        case 1000000:
            strcpy(mode_str, "baud=1000000");
            break;
        case 1500000:
            strcpy(mode_str, "baud=1500000");
            break;
        case 2000000:
            strcpy(mode_str, "baud=2000000");
            break;
        case 3000000:
            strcpy(mode_str, "baud=3000000");
            break;
        default:
            return INVALID_BAUDRATE;
            break;
        }
        strcat(mode_str, " data=8 parity=n stop=1 xon=off to=off odsr=off dtr=on rts=on");

        file_id[port] = CreateFileA(DEVICE_FILE[port], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (file_id[port] == INVALID_HANDLE_VALUE)
            return UNABLE_TO_OPEN_PORT;

        DCB port_settings;
        memset(&port_settings, 0, sizeof(port_settings)); // TODO: remove memset
        port_settings.DCBlength = sizeof(port_settings);

        if (!BuildCommDCBA(mode_str, &port_settings)) {
            CloseHandle(file_id[port]);
            return UNABLE_TO_SET_PORT_SETTINGS;
        }

        // check
        port_settings.fOutxCtsFlow = TRUE;
        port_settings.fRtsControl = RTS_CONTROL_HANDSHAKE;

        if (!SetCommState(file_id[port], &port_settings)) {
            CloseHandle(file_id[port]);
            return UNABLE_TO_GET_PORT_SETTINGS;
        }

        COMMTIMEOUTS timeouts;

        timeouts.ReadIntervalTimeout = MAXDWORD;
        timeouts.ReadTotalTimeoutMultiplier = 0;
        timeouts.ReadTotalTimeoutConstant = 0;
        timeouts.WriteTotalTimeoutMultiplier = 0;
        timeouts.WriteTotalTimeoutConstant = 0;

        if (!SetCommTimeouts(file_id[port], &timeouts)) {
            CloseHandle(file_id[port]);
            return UNABLE_TO_GET_PORT_STATUS;
        }

        return SUCCESS;
    }
    void write_buffer(const unsigned int port, const void *data, const unsigned int size) {
        int passthrough;
        WriteFile(file_id[port], data, size, reinterpret_cast<LPDWORD>(&passthrough), NULL);
    }
    void read_device(const unsigned int port, void *data, const unsigned int size) {
        int passthrough;
        ReadFile(file_id[port], data, size, reinterpret_cast<LPDWORD>(&passthrough), NULL);
    }
#endif
    int open_device_err(const unsigned int port, const unsigned int baudrate) {
        switch (open_device(port, baudrate)) {
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
