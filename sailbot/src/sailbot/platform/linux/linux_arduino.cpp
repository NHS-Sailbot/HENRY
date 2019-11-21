#include "../../core/arduino.hpp"

#define DEBUG_ENABLE_TIMING
#define DEBUG_ENABLE_LOGGING
#include <debug.hpp>

namespace sailbot { namespace plat_linux {
    static constexpr unsigned int HANDSHAKE_ATTEMPTS = 10, TICKS_PER_SECOND = 10;
    static constexpr double TICK_INTERVAL = 1.0 / TICKS_PER_SECOND;
    static void default_update(Arduino &) {}

    bool Arduino::open_device(const char *const filepath) {
        DEBUG_BEGIN_FUNC_PROFILE;

        if (m_serial_device.open(filepath, baudrate)) {
            debug::log::message("Attempting to shake hands with %s...", filepath);

            for (unsigned int i = 0; i < HANDSHAKE_ATTEMPTS; ++i) {
                debug::timer::sleep(TICK_INTERVAL);
                m_serial_device.write(m_tdata_buffer, m_tsize);
                debug::timer::sleep(TICK_INTERVAL);
                m_serial_device.read(m_rdata_buffer, m_rsize);

                for (unsigned int i = 0; i < m_rsize; ++i)
                    debug::log::info("%0*x ", 2, m_rdata_buffer[i]);
                debug::log::info(": %0*x\n", 2, key);

                if (m_rdata_buffer[0] == key && m_rdata_buffer[m_rsize - 1] == key) {
                    debug::log::success("Opened device %s at baudrate %d, using key %x.", filepath, baudrate, key);
                    m_tick_begin = debug::timer::now();
                    is_valid = true;
                    return true;
                }
            }
            m_serial_device.close();
        }
        is_valid = false;
        return false;
    }
    Arduino::Arduino(const unsigned int rsize, const unsigned int tsize, const unsigned int baudrate, const unsigned char key)
        : m_rsize(rsize + 2), m_tsize(tsize + 2), m_rdata_buffer(new unsigned char[rsize + 2]),
          m_tdata_buffer(new unsigned char[tsize + 2]), m_on_update(default_update), is_valid(false), key(key),
          baudrate(baudrate), rdata_handle(m_rdata_buffer + 1), tdata_handle(m_tdata_buffer + 1) {
        DEBUG_BEGIN_FUNC_PROFILE;

        char filepath[] = "/dev/ttyACM0";
        for (; filepath[11] < '1' + 1; ++filepath[11])
            if (open_device(filepath)) return;
        filepath[9] = 'M', filepath[10] = 'A', filepath[11] = '0';
        for (; filepath[11] < '1' + 1; ++filepath[11])
            if (open_device(filepath)) return;
        filepath[8] = 'U', filepath[9] = 'S', filepath[10] = 'B', filepath[11] = '0';
        for (; filepath[11] < '5' + 1; ++filepath[11])
            if (open_device(filepath)) return;
        filepath[8] = 'S', filepath[9] = '0', filepath[10] = '\0';
        for (; filepath[9] < '9' + 1; ++filepath[9])
            if (open_device(filepath)) return;
    }
    Arduino::~Arduino() {
        DEBUG_BEGIN_FUNC_PROFILE;

        delete m_rdata_buffer, m_tdata_buffer;
    }
    void Arduino::update() {
        const double current_time = debug::timer::now();
        if (current_time - m_tick_begin > TICK_INTERVAL) {
            DEBUG_BEGIN_FUNC_PROFILE;

            switch (m_tick) {
            case 0:
                m_serial_device.write(m_tdata_buffer, m_tsize);
                m_tick = 1;
                break;
            case 1:
                m_serial_device.read(m_rdata_buffer, m_rsize);
                if (m_rdata_buffer[0] == key && m_rdata_buffer[m_rsize - 1] == key) {
                    m_tick_begin = debug::timer::now();
                    is_valid = true;
                } else {
                    debug::log::error("failed to find the key (%x) when reading the device.", key);
                    is_valid = false;
                }
                DEBUG_BEGIN_PROFILE(User_defined_update_function);
                m_on_update(*this);
                DEBUG_END_PROFILE(User_defined_update_function);
                m_tick = 0;
                break;
            }

            m_tick_begin += TICK_INTERVAL;
        }
    }
}} // namespace sailbot::plat_linux
