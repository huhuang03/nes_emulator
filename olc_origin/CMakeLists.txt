file(GLOB_RECURSE olc_origin_SRC *.cpp *.h)

add_executable(olc_origin ${olc_origin_SRC})

file(GLOB_RECURSE olc_origin_lib_SRC *.cpp *.h)
list(FILTER olc_origin_lib_SRC EXCLUDE REGEX ".*/olcNes_PPU.*.cpp$")

add_library(olc_origin_lib ${olc_origin_lib_SRC})
