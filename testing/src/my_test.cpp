#include <sailbot.hpp>

#include <iostream>
#include <unistd.h>

int main() {
    constexpr unsigned int PORT = 24;
    sailbot::comm::open_device_err(PORT);

    struct Data {
        char letters[4];
        float number;
    };

    Data my_data = {'a', 'b', 'c', 'd', 15.f};
    Data read_data;

    std::cout << "Waiting for connection to 'warm up'\n";
    usleep(1000000);
    std::cout << "Starting data transmission\n";
    while (1) {
        std::cout << "sent!" << std::endl;
        sailbot::comm::write_buffer(PORT, &my_data, sizeof(Data));
        usleep(1000000);
        // sailbot::comm::read_device(PORT, &read_data, sizeof(Data));
        // std::cout << "read: " << read_data.number << std::endl;
        // usleep(1000000);
    }
    return 0;
}
