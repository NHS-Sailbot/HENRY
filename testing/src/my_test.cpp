#include <sailbot.hpp>

struct TData {
    char bytes[9];
    unsigned short words[3];
    float floats[3];
};

struct RData {
    unsigned short words[3];
    char bytes[8];
};

TData tdata = {"Computer", {7, 8, 9}, {0.4, 0.5, 0.6}};
RData rdata;

int main() {
    sailbot::system::init();

    while (sailbot::system::update()) {
        if (sailbot::system::should_tick()) {
            sailbot::system::handshake(&tdata, sizeof(TData), &rdata, sizeof(RData));
        }
    }
}
