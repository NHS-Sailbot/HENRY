cmake_minimum_required(VERSION 3.6)

# project name
project(template)

# includes
set(DEBUG_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/debug/src")
set(SAILBOT_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/sailbot/src")

# binaries
add_subdirectory(debug)
add_subdirectory(sailbot)
add_subdirectory(main)
add_subdirectory(test)

# move ALL_BUILD, ZERO_CHECK, INSTALL, 
# PACKAGE and RUN_TESTS into their own folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# set the default startup executable to main
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
