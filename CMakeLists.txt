cmake_minimum_required(VERSION 3.12)

project(nes)

FILE(GLOB_RECURSE SRC *.h *.cc)

add_executable(nes ${SRC})
