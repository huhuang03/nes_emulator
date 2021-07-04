FILE(GLOB_RECURSE nes_lib_SRC *.h *.cc)

add_library(nes_lib ${nes_lib_SRC})
target_include_directories(nes_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# how to define a export lib?