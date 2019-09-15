#pragma once

namespace sailbot { namespace arduino {
    // TODO: Documentation Comment
    bool connect(const char *const filepath, const unsigned int baudrate);

    // TODO: Documentation Comment
    void transmit(const void *const data, const unsigned int size);

    // TODO: Documentation Comment
    void receive(void *const data, const unsigned int size);

    // TODO: Documentation Comment
    void disconnect();
}} // namespace sailbot::arduino
