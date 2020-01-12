
add_executable(master main.cpp)

target_link_libraries(master arduino1.0.0 serial1.0.0 camera1.0.0 debug1.0.0 engine1.0.0 jpeg glfw glad stb)
