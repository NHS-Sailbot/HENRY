#include <iostream>
#include <sailbot.hpp>

struct Data {
    char letters[4];
    unsigned int integer;
    float decimal;
};

int main() {
    constexpr unsigned int PORT = 2;
    if (sailbot::comm::open_device_err(PORT, 57600))
        return 1;
    Data my_data = {"abc", 10, 1.5f};
    Data read_data;

    std::cout << "Waiting for connection to 'warm up'\n";
    sailbot::timing::sleep(1.0);
    std::cout << "Starting data transmission\n";
    while (1) {
        std::cout << "sent!" << std::endl;
        sailbot::comm::write_buffer(PORT, &my_data, sizeof(my_data));
        sailbot::timing::sleep(1.0);
        sailbot::comm::read_device(PORT, &read_data, sizeof(my_data));
        std::cout << "read: {" << read_data.letters << ' ' << read_data.integer << ' ' << read_data.decimal << '}' << std::endl;
        sailbot::timing::sleep(1.0);
    }
    return 0;
}
