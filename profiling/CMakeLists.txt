project(profiling)

set(EXECUTABLE_NAME MVMFilterProfiling)
add_executable(${EXECUTABLE_NAME} Profiling.cpp)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE MVMFilterLib)