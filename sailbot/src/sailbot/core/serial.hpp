#pragma once

namespace sailbot { namespace plat_linux {
    class SerialDevice {
        int m_file_descriptor;
        bool m_is_currently_open;
        const char *m_filepath;
        unsigned int m_baudrate;

      public:
        SerialDevice() = default;
        SerialDevice(const char *const filepath, const unsigned int baudrate);
        ~SerialDevice();
        bool open(const char *const filepath, const unsigned int baudrate);
        void close();
        void flush();
        void read(void *const buffer, const unsigned int size) const;
        void write(void *const buffer, const unsigned int size) const;
        bool is_open() const;
    };
}} // namespace sailbot::plat_linux
