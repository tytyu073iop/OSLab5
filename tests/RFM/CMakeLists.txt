cmake_minimum_required(VERSION 4.0)

project(RFMTest)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#test
enable_testing()

add_executable(RFMTest test.cpp)
target_link_libraries(RFMTest RFM)
target_link_libraries(RFMTest GTest::gtest_main)
set_property(TARGET RFMTest PROPERTY CXX_STANDARD 20)
target_compile_features(RFMTest PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(RFMTest)