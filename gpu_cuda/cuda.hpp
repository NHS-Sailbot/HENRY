#pragma once

namespace cuda {
    void init();
    void perform_kernel(unsigned char *const p);
    void deinit();
} // namespace cuda
