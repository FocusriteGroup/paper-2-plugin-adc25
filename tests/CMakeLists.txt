project(tests)

set(EXECUTABLE_NAME MVMFilterTests)
add_executable(${EXECUTABLE_NAME} Tests.cpp)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE MVMFilterLib)
add_custom_command(TARGET ${EXECUTABLE_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${EXECUTABLE_NAME}>" "${CMAKE_CURRENT_SOURCE_DIR}/$<TARGET_FILE_NAME:${EXECUTABLE_NAME}>")