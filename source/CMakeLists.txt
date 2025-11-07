project(MVMFilterLib)

set(LIBRARY_NAME MVMFilterLib)

add_library(${LIBRARY_NAME} STATIC MVMFilter.cpp)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})