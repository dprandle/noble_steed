project(samples)

file(GLOB BASIC_WINDOW_SRC basic_window/*.cpp)
file(GLOB COMP_AND_ENTITY_SRC comp_and_entity/*.cpp)
file(GLOB COPY_ASSIGNMENT_SRC copy_assignment/*.cpp)
file(GLOB PACKAGE_TESTING_SRC package_testing/*.cpp)


if(WIN32)
elseif(APPLE)
add_executable(Basic_Window MACOSX_BUNDLE ${BASIC_WINDOW_SRC})
add_executable(Comp_And_Entity MACOSX_BUNDLE ${COMP_AND_ENTITY_SRC})
add_executable(Copy_Assignment MACOSX_BUNDLE ${COPY_ASSIGNMENT_SRC})
add_executable(Package_Testing MACOSX_BUNDLE ${PACKAGE_TESTING_SRC})

set_target_properties(Basic_Window PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PLIST_FILE})
set_target_properties(Basic_Window PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Basic_Window")
set_target_properties(Basic_Window PROPERTIES MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.Basic_Window")

set_target_properties(Comp_And_Entity PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PLIST_FILE})
set_target_properties(Comp_And_Entity PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Comp_And_Entity")
set_target_properties(Comp_And_Entity PROPERTIES MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.Comp_And_Entity")

set_target_properties(Copy_Assignment PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PLIST_FILE})
set_target_properties(Copy_Assignment PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Copy_Assignment")
set_target_properties(Copy_Assignment PROPERTIES MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.Copy_Assignment")

set_target_properties(Package_Testing PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PLIST_FILE})
set_target_properties(Package_Testing PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Package_Testing")
set_target_properties(Package_Testing PROPERTIES MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.Package_Testing")
else()
add_executable(Basic_Window ${BASIC_WINDOW_SRC})
add_executable(Comp_And_Entity ${COMP_AND_ENTITY_SRC})
add_executable(Copy_Assignment ${COPY_ASSIGNMENT_SRC})
add_executable(Package_Testing ${PACKAGE_TESTING_SRC})
endif()

target_link_libraries(Basic_Window ${TARGET_NAME})
target_link_libraries(Comp_And_Entity ${TARGET_NAME})
target_link_libraries(Copy_Assignment ${TARGET_NAME})
target_link_libraries(Package_Testing ${TARGET_NAME})
