FILE(GLOB_RECURSE nes_lib_SRC *.h *.cc)

add_library(nes_lib ${nes_lib_SRC})