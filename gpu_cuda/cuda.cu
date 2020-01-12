#include "cuda.hpp"

static constexpr unsigned int 
    PIXEL_SIZE = 3 * sizeof(unsigned char), 
    WIDTH = 1280, HEIGHT = 720, 
    PIXEL_COUNT = WIDTH * HEIGHT, 
    DATA_SIZE = PIXEL_COUNT * PIXEL_SIZE;

static constexpr dim3 BLOCK_SIZE(10, 10), THREAD_SIZE(128, 72);

__global__ void pixel_kernel(unsigned char *const out, unsigned char *const pixel) {
    const unsigned int x = blockIdx.x * THREAD_SIZE.x + threadIdx.x;
    const unsigned int y = blockIdx.y * THREAD_SIZE.y + threadIdx.y;
    const unsigned int index = (x + y * WIDTH) * PIXEL_SIZE;
    // if (index < DATA_SIZE - 2) {
        out[index + 0] = 255;
        out[index + 1] = 0;
        out[index + 2] = 255;
    // }
}

namespace cuda {
    static unsigned char *data1 = nullptr, *data2 = nullptr;

    void init() {
        cudaMalloc(&data1, DATA_SIZE);
        cudaMalloc(&data2, DATA_SIZE);
    }
    void perform_kernel(unsigned char *const p) {
        cudaMemcpy(data1, p, DATA_SIZE, cudaMemcpyHostToDevice);
        cudaMemcpy(data2, p, DATA_SIZE, cudaMemcpyHostToDevice);
        pixel_kernel<<<BLOCK_SIZE, THREAD_SIZE>>>(data1, data2);
        cudaMemcpy(p, data1, DATA_SIZE, cudaMemcpyDeviceToHost);
        // for (unsigned int i = 0; i < DATA_SIZE; ++i) p[i] = 100;
    }
    void deinit() {
        cudaFree(data1);
        cudaFree(data2);
    }
}
