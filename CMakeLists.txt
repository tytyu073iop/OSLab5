cmake_minimum_required(VERSION 4.0)

project(OSLab5)

set(CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD 20)


add_library(RFM RFM.cpp)
target_include_directories(RFM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(RFM PRIVATE cxx_std_20)

add_subdirectory(tests/RFM)