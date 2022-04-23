cmake_minimum_required(VERSION 3.16.0)

project(noble_steed)

set(CMAKE_POLICY_DEFAULT_CMP0072 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(TARGET_NAME noble_steed)
set(PYTHON_MODULE_NAME Noble_Steed)

# Some basica CMAKE values
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(PLIST_FILE ${CMAKE_SOURCE_DIR}/cmake/Noble_Steed.plist)

# Dependency directory
set(DEPENDENCY_DIR ${CMAKE_SOURCE_DIR}/deps)

# Set dependency directories
set(GLFW_DIR ${DEPENDENCY_DIR}/glfw)
set(BGFX_DIR ${DEPENDENCY_DIR}/bgfx)
set(BX_DIR ${DEPENDENCY_DIR}/bx)
set(BIMG_DIR ${DEPENDENCY_DIR}/bimg)
set(ASSIMP_DIR ${DEPENDENCY_DIR}/assimp)
set(BULLET_DIR ${DEPENDENCY_DIR}/bullet3)
set(PYBIND_DIR ${DEPENDENCY_DIR}/pybind11)

# Set library source directory
set(NOBLE_STEED_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Set include directories
set(BGFX_INCLUDE_DIR ${BGFX_DIR}/include)
set(GLFW_INCLUDE_DIR ${GLFW_DIR}/include)
set(BX_INCLUDE_DIR ${BX_DIR}/include)
set(BIMG_INCLUDE_DIR ${BIMG_DIR}/include)
set(BULLET_INCLUDE_DIR ${BULLET_DIR}/src)
set(PYBIND_INCLUDE_DIR ${PYBIND_DIR}/include)
set(ASSIMP_INCLUDE_DIR ${ASSIMP_DIR}/assimp)

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

add_definitions(
    -DBX_CONFIG_DEBUG
)

# Debug and release specific settings
if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
  add_definitions(-DDEBUG_BUILD)
  add_definitions(-DDBG_MACRO_NO_WARNING)
else()
  add_definitions(-DRELEASE_BUILD)
  add_definitions(-DNDEBUG)
endif()
add_definitions(-D_LIBCPP_NO_EXPERIMENTAL_DEPRECATION_WARNING_FILESYSTEM)

# Set the src files for the project
file(GLOB SRC_FILES 
  ${NOBLE_STEED_SRC_DIR}/*.cpp
  ${NOBLE_STEED_SRC_DIR}/*.c
  ${NOBLE_STEED_SRC_DIR}/math/*.cpp
  )

# Set project includes dir
include_directories(
  ${BGFX_INCLUDE_DIR}
  ${BX_INCLUDE_DIR}
  ${BIMG_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
  ${PYBIND_INCLUDE_DIR}
  ${ASSIMP_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  )
  
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tools)
add_subdirectory(${GLFW_DIR})
add_subdirectory(${ASSIMP_DIR})
add_subdirectory(${BULLET_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if (WIN32)
    # Windows stuff here
    add_definitions(-DPLATFORM_WINDOWS)
    add_definitions(-DGLFW_EXPOSE_NATIVE_WIN32)
elseif(APPLE)
    add_definitions(-DPLATFORM_OSX)
    add_definitions(-DGLFW_EXPOSE_NATIVE_COCOA)
    set(CMAKE_MACOSX_RPATH 1)
    
    # Set dependency library paths
    set(BGFX_LIB_NAME libbgfx-shared-lib${CMAKE_BUILD_TYPE}.dylib)
    set(BGFX_BIN_DIR "${BGFX_DIR}/.build/osx-x64/bin")
    set(BGFX_LIB_PATH "${BGFX_BIN_DIR}/${BGFX_LIB_NAME}")
    message("BGFX: ${BGFX_BIN_DIR}/${BGFX_LIB_NAME}")
    
    link_directories(/usr/local/lib)
    include_directories(/usr/local/include)    
    add_executable(${TARGET_NAME} ${SRC_FILES})
    target_link_libraries(${TARGET_NAME}
        ${BGFX_LIB_PATH}  
        glfw
        stdc++
        assimp
        Bullet2FileLoader
        Bullet3Dynamics
        Bullet3OpenCL_clew
        Bullet3Collision
        BulletInverseDynamics
        Bullet3Geometry
        BulletSoftBody
        BulletCollision
        Bullet3Common
        BulletDynamics)

    set_target_properties(${TARGET_NAME} PROPERTIES FRAMEWORK TRUE)
else()
    add_definitions(-DPLATFORM_LINUX)
    add_definitions(-DGLFW_EXPOSE_NATIVE_X11)
    add_executable(${TARGET_NAME} ${SRC_FILES})

    # Set dependency library paths
    set(BGFX_LIB_NAME libbgfx-shared-lib${CMAKE_BUILD_TYPE}.so)
    set(BGFX_BIN_DIR "${BGFX_DIR}/.build/linux64_gcc/bin")
    set(BGFX_LIB_PATH "${BGFX_BIN_DIR}/${BGFX_LIB_NAME}")
  
    target_link_libraries(${TARGET_NAME}
        ${BGFX_LIB_PATH}
        glfw
        assimp
        Bullet2FileLoader
        Bullet3Dynamics
        Bullet3OpenCL_clew
        Bullet3Collision
        BulletInverseDynamics
        Bullet3Geometry
        BulletSoftBody
        BulletCollision
        Bullet3Common
        BulletDynamics
        ${PYTHON_LIBRARIES}
        X11
        GL
        stdc++fs
        pthread)
endif()

if(MSVC)
    target_compile_options(${TARGET_NAME} PUBLIC /arch:SSE4.1)
else()
    target_compile_options(${TARGET_NAME} PUBLIC -msse4.1)
endif()

configure_file(${BGFX_BIN_DIR}/shaderc${CMAKE_BUILD_TYPE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools/shaderc COPYONLY)
configure_file(${BGFX_BIN_DIR}/texturec${CMAKE_BUILD_TYPE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools/texturec COPYONLY)
configure_file(${BGFX_BIN_DIR}/texturev${CMAKE_BUILD_TYPE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools/texturev COPYONLY)
configure_file(${BGFX_BIN_DIR}/geometryc${CMAKE_BUILD_TYPE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools/geometryc COPYONLY)
configure_file(${BGFX_BIN_DIR}/geometryv${CMAKE_BUILD_TYPE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools/geometryv COPYONLY)

# Copy the shader lib source to the default shader lib directory in build folder
# message("Trying to copy " ${CMAKE_SOURCE_DIR}/data " to " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/shaders)
# add_custom_target(deploy_shader_lib ALL
#  COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/shader_lib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/shader_lib
#  DEPENDS ${TARGET_NAME}
#  )


# pybind11_add_module(${PYTHON_MODULE_NAME} ${CMAKE_SOURCE_DIR}/src/core/python_bindings.cpp)