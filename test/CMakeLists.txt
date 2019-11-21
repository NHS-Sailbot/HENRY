include_directories("${DEBUG_INCLUDE_PATH}")
include_directories("${SAILBOT_INCLUDE_PATH}")

# set file sources
file(GLOB_RECURSE TEST_SOURCES "src/*.cpp" "src/*.hpp")

# executable name and sources
add_executable(test ${TEST_SOURCES})

target_link_libraries(test general debug_lib sailbot_lib pthread)
