file(GLOB LIB_SRC "../src/*.cc" "../src/*.h")

list(FILTER LIB_SRC EXCLUDE REGEX ".*/main.cc$")

add_executable(cpu_debugger cpu_debugger.cc cpu_debugger.h ${LIB_SRC})
add_executable(cpu_simulator cpu_simulator.cc cpu_simulator.h ${LIB_SRC})
message("Cmake Current Source DIR: " ${CMAKE_CURRENT_SOURCE_DIR})
# not work??
target_include_directories(cpu_simulator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/)
target_include_directories(cpu_debugger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/)

