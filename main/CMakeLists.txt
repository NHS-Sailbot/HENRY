include_directories("${DEBUG_INCLUDE_PATH}")
include_directories("${SAILBOT_INCLUDE_PATH}")

# set file sources
file(GLOB_RECURSE MAIN_SOURCES "src/*.cpp" "src/*.hpp")

# executable name and sources
add_executable(main ${MAIN_SOURCES})

target_link_libraries(main general debug_lib sailbot_lib)
