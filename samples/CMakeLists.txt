project(samples)

add_executable(Window basic_window.cpp)

#if (UNIX)
target_link_libraries(Window pthread)
target_link_libraries(Window X11)
target_link_libraries(Window dl)
target_link_libraries(Window GL)
#endif()

target_link_libraries(Window ${TARGET_NAME})