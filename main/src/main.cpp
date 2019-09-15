#include <sailbot.hpp>

int main() {
    sailbot::system::init({9600});

    while (!sailbot::system::should_close()) {

        // Your sail logic here

        sailbot::system::update();
    }

    sailbot::system::shutdown();
}
