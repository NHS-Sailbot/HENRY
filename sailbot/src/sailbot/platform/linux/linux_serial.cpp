#include "../../core/serial.hpp"

#define DEBUG_ENABLE_TIMING
#define DEBUG_ENABLE_LOGGING
#include <debug.hpp>

#include <errno.h>
#include <string.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace sailbot { namespace plat_linux {
    static inline constexpr unsigned int convert_baudrate(const unsigned int baudrate) {
        switch (baudrate) {
        case 50: return B50;
        case 75: return B75;
        case 110: return B110;
        case 134: return B134;
        case 150: return B150;
        case 200: return B200;
        case 300: return B300;
        case 600: return B600;
        case 1200: return B1200;
        case 1800: return B1800;
        case 2400: return B2400;
        case 4800: return B4800;
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
        case 500000: return B500000;
        case 576000: return B576000;
        case 921600: return B921600;
        case 1000000: return B1000000;
        case 1152000: return B1152000;
        case 1500000: return B1500000;
        case 2000000: return B2000000;
        case 2500000: return B2500000;
        case 3000000: return B3000000;
        case 3500000: return B3500000;
        case 4000000: return B4000000;
        default: return -1;
        }
    }

    SerialDevice::SerialDevice(const char *const filepath, const unsigned int baudrate)
        : m_filepath(filepath), m_baudrate(baudrate) {
        DEBUG_BEGIN_FUNC_PROFILE;

        open(filepath, baudrate);
    }
    SerialDevice::~SerialDevice() { close(); }
    bool SerialDevice::open(const char *const filepath, const unsigned int baudrate) {
        DEBUG_BEGIN_FUNC_PROFILE;

        // More information about serial communication can be found at
        // https://www.cmrr.umn.edu/~strupp/serial.html#2_1. This is where
        // I based this code off of.

        // Attempt to open the file.
        m_file_descriptor = ::open(filepath, O_RDWR | O_NOCTTY | O_NDELAY);
        if (m_file_descriptor == -1) {
            debug::log::message("Device::open(): Unable to open %s: %s", filepath, strerror(errno));
            m_is_currently_open = false;
            return false;
        } else {
            fcntl(m_file_descriptor, F_SETFL, 0);
            m_is_currently_open = true;
        }

        // Lock access so that another process can't also use the port.
        if (flock(m_file_descriptor, LOCK_EX | LOCK_NB) != 0) {
            close();
            return false;
        }

        // If one requires no blocking, uncomment
        // fcntl(file_descriptor, F_SETFL, FNDELAY);

        // Setting the port options
        struct termios port_settings, prev_port_settings;

        // First get the current options
        tcgetattr(m_file_descriptor, &port_settings);
        prev_port_settings = port_settings;

        // Set the baudrate into the options (in and out)
        cfsetispeed(&port_settings, convert_baudrate(baudrate));
        cfsetospeed(&port_settings, convert_baudrate(baudrate));

        // CLOCAL restricts the operating system from changing the owner of the
        // port to be this program, and CREAD enables the reciever.
        // Both of these should always be set.
        port_settings.c_cflag |= (CLOCAL | CREAD);

        // Disable parity bits
        // options.c_cflag &= ~PARENB; // disable parity
        // options.c_cflag &= ~CSTOPB; // disable 2 stop bits

        // Enable hardware flow control. This is not supported on all platforms.
        // Some systems may define it as CNEW_RTSCTS, where others define it as CRTSCTS.
        // options.c_cflag |= CRTSCTS;

        port_settings.c_cflag &= ~CSIZE; // Mask the character size bits
        port_settings.c_cflag |= CS8;    // Select 8 data bits

        // Enable parity checking on the incoming stream as well as strip the parity bits.
        port_settings.c_iflag |= (INPCK | ISTRIP);

        // TODO: Read more up on these port settings.
        port_settings.c_iflag = IGNPAR;
        port_settings.c_oflag = 0;
        port_settings.c_lflag = 0;
        port_settings.c_cc[VMIN] = 0;  // block untill n bytes are received
        port_settings.c_cc[VTIME] = 0; // block untill a timer expires (n * 100 mSec.)

        //

        int result = tcsetattr(m_file_descriptor, TCSANOW, &port_settings);

        if (result == -1) {
            debug::log::message("Device::open(): Unable to open %s: %s", filepath, strerror(errno));
            tcsetattr(m_file_descriptor, TCSANOW, &prev_port_settings);
            close();
            return false;
        }

        int status;
        if (ioctl(m_file_descriptor, TIOCMGET, &status) == -1) {
            debug::log::message("Device::open(): Unable to open %s: %s", filepath, strerror(errno));
            tcsetattr(m_file_descriptor, TCSANOW, &prev_port_settings);
            close();
            return false;
        }

        status |= TIOCM_DTR; // turn on DTR
        status |= TIOCM_RTS; // turn on RTS

        if (ioctl(m_file_descriptor, TIOCMSET, &status) == -1) {
            debug::log::message("Device::open(): Unable to open %s: %s", filepath, strerror(errno));
            tcsetattr(m_file_descriptor, TCSANOW, &prev_port_settings);
            close();
            return false;
        }

        debug::timer::sleep(2);

        tcflush(m_file_descriptor, TCIOFLUSH);

        return true;
    }
    void SerialDevice::close() {
        DEBUG_BEGIN_FUNC_PROFILE;

        if (m_is_currently_open) {
            flock(m_file_descriptor, LOCK_UN); // Free the port so that others can use it.
            ::close(m_file_descriptor);        // Close the device file.
            m_is_currently_open = false;
        }
    }
    void SerialDevice::flush() {
        DEBUG_BEGIN_FUNC_PROFILE;

        if (m_is_currently_open) {
            tcflush(m_file_descriptor, TCIOFLUSH); // Flush the io buffers
        }
    }
    void SerialDevice::read(void *const buffer, const unsigned int size) const {
        DEBUG_BEGIN_FUNC_PROFILE;

        // Before reading from the device, make sure the device has been opened.
        if (m_is_currently_open) {
            // read directly from the device file.
            int bytes_read = ::read(m_file_descriptor, buffer, size);
            if (bytes_read < 0) debug::log::error("read() of %d bytes failed!", size);
        }
    }
    void SerialDevice::write(void *const buffer, const unsigned int size) const {
        DEBUG_BEGIN_FUNC_PROFILE;

        // Before writing to the device, make sure the device has been opened.
        if (m_is_currently_open) {
            // Write directly to the device file.
            int bytes_written = ::write(m_file_descriptor, buffer, size);
            // Make sure that the number of bytes written isn't less than 0.
            // This is true when it fails to write to the file.
            if (bytes_written < 0) debug::log::error("write() of %d bytes failed!", size);
        }
    }
    bool SerialDevice::is_open() const {
        DEBUG_BEGIN_FUNC_PROFILE;

        return m_is_currently_open;
    }
}} // namespace sailbot::plat_linux
