#pragma once

namespace sailbot { namespace system {
    int init();
    bool update(const void *tdata, const unsigned int tsize, void *rdata, const unsigned int rsize);
}} // namespace sailbot::system
