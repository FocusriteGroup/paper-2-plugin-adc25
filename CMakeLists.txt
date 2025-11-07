cmake_minimum_required (VERSION VERSION 3.28 FATAL_ERROR)

project ("workshop-adc25")

set (CMAKE_CXX_STANDARD 20)

add_subdirectory(profiling)
add_subdirectory(source)
add_subdirectory(tests)