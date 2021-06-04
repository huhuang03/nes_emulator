cmake_minimum_required(VERSION 3.12)

project(nes)

# copy the assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

FILE(GLOB_RECURSE NES_SRC *.h *.cc)

# we need the full path.
#list(FILTER NES_SRC EXCLUDE REGEX ".*cpu_debugger.(cc|h)$")
#add_executable(nes ${NES_SRC})


#FILE(GLOB_RECURSE NES_DEBUGGER_SRC *.h *.cc)
#message(${NES_DEBUGGER_SRC})
#list(FILTER NES_DEBUGGER_SRC EXCLUDE REGEX ".*/main.cc$")
#add_executable(nes_debugger ${NES_DEBUGGER_SRC})
#
add_subdirectory(demos)