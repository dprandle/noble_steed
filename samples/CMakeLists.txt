project(samples)

add_executable(Basic_Window basic_window.cpp)
add_executable(Comp_And_Entity comp_and_entity.cpp)

target_link_libraries(Basic_Window ${TARGET_NAME})
target_link_libraries(Comp_And_Entity ${TARGET_NAME})


#if (UNIX)
target_link_libraries(Basic_Window RTTR::Core)
target_link_libraries(Comp_And_Entity RTTR::Core)
#target_link_libraries(Comp_And_Entity X11)
#target_link_libraries(Comp_And_Entity dl)
#target_link_libraries(Comp_And_Entity GL)
#endif()

