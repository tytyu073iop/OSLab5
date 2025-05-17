cmake_minimum_required(VERSION 4.0)

project(OSLab5)

set(CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD 20)


add_library(RFM RFM.cpp)
target_include_directories(RFM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(RFM PRIVATE cxx_std_20)

add_library(globals globals.cpp)
target_include_directories(globals PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(thread thread.cpp)
target_include_directories(thread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.cpp)
target_link_libraries(main PUBLIC RFM globals thread)
target_compile_features(main PUBLIC cxx_std_20)

add_executable(client client.cpp)
target_link_libraries(client PUBLIC RFM globals thread)
target_compile_features(client PUBLIC cxx_std_20)

add_subdirectory(tests/RFM)