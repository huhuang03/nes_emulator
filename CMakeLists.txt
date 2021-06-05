cmake_minimum_required(VERSION 3.12)

project(nes)

# copy the assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
FILE(GLOB_RECURSE NES_SRC *.h *.cc)
add_subdirectory(demos)

# now let include test