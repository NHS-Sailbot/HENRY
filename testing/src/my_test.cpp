#include <iostream>
#include <sailbot.hpp>

struct Data {
    char letters[4];
    unsigned int integer;
    double decimals[10];
};

double randd() { return static_cast<double>(rand() % 100) / 100; }
void print_data(const Data &d) {
    std::cout << d.letters << ' ' << d.integer << ' ';
    for (unsigned int i = 0; i < 10; ++i)
        std::cout << d.decimals[i] << ' ';
}

int main() {
    constexpr unsigned int PORT = 2;
    if (sailbot::comm::open_device_err(PORT, 57600))
        return 1;
    const Data send_data = {"abc", 10, {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9}};
    Data read_data;

    std::cout << "Waiting for connection to 'warm up'\n";
    sailbot::timing::sleep(1.0);
    std::cout << "Starting data transmission\n";
    while (1) {
        std::cout << "sent: {";
        print_data(send_data);
        std::cout << '}' << std::endl;
        sailbot::comm::write_buffer(PORT, &send_data, sizeof(send_data));
        sailbot::timing::sleep(0.1);
        sailbot::comm::read_device(PORT, &read_data, sizeof(read_data));
        std::cout << "read: {";
        print_data(read_data);
        std::cout << '}' << std::endl;
        sailbot::timing::sleep(0.1);
    }
    return 0;
}
