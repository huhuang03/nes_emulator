# how to let include all file under ../src?
file(GLOB_RECURSE DEMO_SRC *.cc *.h)

list(FILTER DEMO_SRC EXCLUDE REGEX ".*/cpu_.*.cc$")

list(APPEND DEMO_SRC "./base_app.cc" "./base_app.h")

add_executable(cpu_debugger cpu_debugger.cc cpu_debugger.h ${DEMO_SRC})
add_executable(cpu_simulator cpu_simulator.cc cpu_simulator.h ${DEMO_SRC})

target_link_libraries(cpu_debugger nes_lib)
target_link_libraries(cpu_simulator nes_lib)

# not work??
target_include_directories(cpu_simulator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/)
target_include_directories(cpu_debugger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/)
