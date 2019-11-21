#pragma once

#include "serial.hpp"

namespace sailbot { namespace plat_linux {
    class Arduino {
        SerialDevice m_serial_device;
        const unsigned int m_rsize, m_tsize;
        unsigned char *const m_rdata_buffer, *const m_tdata_buffer;

        double m_tick_begin = 0.0;
        unsigned char m_tick = 0;
        void (*m_on_update)(Arduino &);

        bool open_device(const char *const filepath);

      public:
        bool is_valid;
        const unsigned char key;
        const unsigned int baudrate;
        unsigned char *const rdata_handle, *const tdata_handle;

        Arduino(const unsigned int rsize, const unsigned int tsize, const unsigned int baudrate, const unsigned char key);
        ~Arduino();
        void update();

        inline void on_update(void (*const f)(Arduino &)) { m_on_update = f; }
    };
}} // namespace sailbot::plat_linux
