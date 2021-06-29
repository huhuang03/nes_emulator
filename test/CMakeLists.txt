# add test.
set(CMAKE_CXX_STANDARD  11)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(
        test_cpu
        test_cpu.cc
)
target_link_libraries(
        test_cpu
        gtest_main
        nes_lib
        olc_origin_lib
)
include(GoogleTest)
gtest_discover_tests(test_cpu)

# What to do?
target_include_directories(test_cpu PRIVATE "../src/")
target_include_directories(test_cpu PRIVATE "../olc_origin")