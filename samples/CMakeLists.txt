project(samples)

if(WIN32)
elseif(APPLE)
add_executable(Basic_Window MACOSX_BUNDLE basic_window.cpp)
add_executable(Comp_And_Entity MACOSX_BUNDLE comp_and_entity.cpp)
set_target_properties(Basic_Window PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/Noble_Steed.plist)
set_target_properties(Comp_And_Entity PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/Noble_Steed.plist)
else()
add_executable(Basic_Window basic_window.cpp)
add_executable(Comp_And_Entity comp_and_entity.cpp)
endif()

target_link_libraries(Basic_Window ${TARGET_NAME})
target_link_libraries(Comp_And_Entity ${TARGET_NAME})

#if (UNIX)
target_link_libraries(Basic_Window RTTR::Core)
target_link_libraries(Comp_And_Entity RTTR::Core)
#target_link_libraries(Comp_And_Entity X11)
#target_link_libraries(Comp_And_Entity dl)
#target_link_libraries(Comp_And_Entity GL)
#endif()

