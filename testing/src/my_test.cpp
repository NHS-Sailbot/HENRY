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

TData tdata = {"Computer", {0.4, 0.5, 0.6}};
RData rdata;

int main() {
    if (sailbot::system::init())
        return 1;

    while (sailbot::system::update(&tdata, sizeof(TData), &rdata, sizeof(RData))) {
        std::cout << rdata.bytes << ' ';
        for (unsigned int i = 0; i < 3; ++i)
            std::cout << rdata.words[i] << ' ';
        std::cout << '\n';
    }
}
