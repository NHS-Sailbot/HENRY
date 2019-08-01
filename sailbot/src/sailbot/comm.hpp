#pragma once

namespace sailbot { namespace comm {
#ifndef _CONFIG_PLATFORM_WINDOWS
    constexpr static unsigned int PORT_COUNT = 32;
    constexpr static const char *DEVICE_FILE[PORT_COUNT] = {
        "/dev/ttyS0",   "/dev/ttyS1",   "/dev/ttyS2",   "/dev/ttyS3",   "/dev/ttyS4",   "/dev/ttyS5",   "/dev/ttyS6",
        "/dev/ttyS7",   "/dev/ttyS8",   "/dev/ttyS9",   "/dev/ttyS10",  "/dev/ttyS11",  "/dev/ttyS12",  "/dev/ttyS13",
        "/dev/ttyS14",  "/dev/ttyS15",  "/dev/ttyAMA0", "/dev/ttyAMA1", "/dev/ttyACM0", "/dev/ttyACM1", "/dev/rfcomm0",
        "/dev/rfcomm1", "/dev/ircomm0", "/dev/ircomm1", "/dev/cuau0",   "/dev/cuau1",   "/dev/cuau2",   "/dev/cuau3",
        "/dev/cuaU0",   "/dev/cuaU1",   "/dev/cuaU2",   "/dev/cuaU3"};
#else
    constexpr static unsigned int PORT_COUNT = 32;
    constexpr static const char *DEVICE_FILE[PORT_COUNT] = {
        "\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",  "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",
        "\\\\.\\COM8",  "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12", "\\\\.\\COM13", "\\\\.\\COM14",
        "\\\\.\\COM15", "\\\\.\\COM16", "\\\\.\\COM17", "\\\\.\\COM18", "\\\\.\\COM19", "\\\\.\\COM20", "\\\\.\\COM21",
        "\\\\.\\COM22", "\\\\.\\COM23", "\\\\.\\COM24", "\\\\.\\COM25", "\\\\.\\COM26", "\\\\.\\COM27", "\\\\.\\COM28",
        "\\\\.\\COM29", "\\\\.\\COM30", "\\\\.\\COM31", "\\\\.\\COM32"};
#endif
    int open_device(const unsigned int port, const unsigned int baudrate);
    int open_device_err(const unsigned int port, const unsigned int baudrate);
    void write_buffer(const unsigned int port, const void *data, const unsigned int size);
    void read_buffer(const unsigned int port, void *data, const unsigned int size);
}} // namespace sailbot::comm
