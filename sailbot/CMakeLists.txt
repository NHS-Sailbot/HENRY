include_directories("${DEBUG_INCLUDE_PATH}")

file(GLOB_RECURSE SAILBOT_SOURCES "src/*.cpp" "src/*.hpp")
add_library(sailbot_lib STATIC ${SAILBOT_SOURCES})

target_link_libraries(sailbot_lib general debug_lib)
