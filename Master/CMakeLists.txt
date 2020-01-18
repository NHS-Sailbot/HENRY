
add_executable(Master Master.cpp)

set(ArduinoDep Arduino Serial Debug)
set(CameraDep Camera jpeg Debug)
set(MasterDep ${ArduinoDep} ${CameraDep})

target_link_libraries(Master ${MasterDep})
