cmake_minimum_required(VERSION 3.12)

project(nes)

FILE(GLOB_RECURSE SRC *.h *.cc)

add_executable(nes ${SRC} src/ppu.cc src/ppu.h src/cartridge.cc src/cartridge.h src/mapper.cc src/mapper.h src/mapper_000.cc src/mapper_000.h)
