cmake_minimum_required(VERSION 3.0.0)
project(henry VERSION 0.1.0)

add_subdirectory(dep/arduino)
add_subdirectory(dep/camera)

link_libraries(arduino camera)
include_directories(dep/arduino/include dep/camera/include dep/camera/dep/debug/include)

add_subdirectory(master)
