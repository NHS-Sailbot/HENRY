
add_executable(cam_test main.cpp ../gpu_cuda/gpu_cuda.o ../gpu_cuda/cuda.o)

target_link_directories(cam_test PRIVATE /usr/local/cuda-10.2/lib64)
target_link_libraries(cam_test arduino1.0.0 serial1.0.0 camera1.0.0 debug1.0.0 jpeg engine1.0.0 glfw glad stb cudart)
