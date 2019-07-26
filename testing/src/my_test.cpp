#include <iostream>
#include <sailbot.hpp>

struct TData {
    char bytes[9];
    float floats[3];
};

struct RData {
    unsigned short words[3];
    char bytes[8];
};

void print_data(const RData &d) {
    std::cout << d.bytes << ' ';
    for (unsigned int i = 0; i < 3; ++i)
        std::cout << d.words[i] << ' ';
    std::cout << '\n';
}

TData tdata = {"Computer", {0.4, 0.5, 0.6}};
RData rdata;

int main() {
    sailbot::system::init();

    while (sailbot::system::update()) {
        if (sailbot::system::should_tick()) {
            sailbot::system::handshake(&tdata, sizeof(TData), &rdata, sizeof(RData));
            print_data(rdata);
        }
    }
}
