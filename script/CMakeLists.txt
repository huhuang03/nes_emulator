project(scritps)

add_executable(dump dump.cc)

target_link_libraries(dump nes_lib)
message(${CMAKE_CURRENT_SOURCE_DIR}/../src/include/)
target_include_directories(dump PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../src/include/")
#target_include_directories(dump nes_lib)
