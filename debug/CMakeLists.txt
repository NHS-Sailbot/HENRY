file(GLOB_RECURSE DEBUG_SOURCES "src/*.cpp" "src/*.hpp")
add_library(debug_lib STATIC ${DEBUG_SOURCES})
