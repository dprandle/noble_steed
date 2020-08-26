cmake_minimum_required(VERSION 3.10.0)

project(noble_steed)

set(TARGET_NAME noble_steed)
set(PYTHON_MODULE_NAME Noble_Steed)

# Some basica CMAKE values
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
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
set(GLM_DIR ${DEPENDENCY_DIR}/glm)
set(ASSIMP_DIR ${DEPENDENCY_DIR}/assimp)
set(BULLET_DIR ${DEPENDENCY_DIR}/bullet3)
set(SPDLOG_DIR ${DEPENDENCY_DIR}/spdlog)
set(MEMORY_ALLOCATORS_DIR ${DEPENDENCY_DIR}/memory-allocators)
set(RTTR_DIR ${DEPENDENCY_DIR}/rttr)
set(PYBIND_DIR ${DEPENDENCY_DIR}/pybind11)

# Set library source directory
set(NOBLE_STEED_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Set include directories
set(NOBLE_STEED_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(BGFX_INCLUDE_DIR ${BGFX_DIR}/include)
set(GLFW_INCLUDE_DIR ${GLFW_DIR}/include)
set(BX_INCLUDE_DIR ${BX_DIR}/include)
set(BIMG_INCLUDE_DIR ${BIMG_DIR}/include)
set(GLM_INCLUDE_DIR ${GLM_DIR})
set(BULLET_INCLUDE_DIR ${BULLET_DIR}/src)
set(SPDLOG_INCLUDE_DIR ${SPDLOG_DIR}/include)
set(MEMORY_ALLOCATORS_INCLUDE_DIR ${MEMORY_ALLOCATORS_DIR}/includes)
set(RTTR_INCLUDE_DIR ${RTTR_DIR}/src)
set(PYBIND_INCLUDE_DIR ${PYBIND_DIR}/include)
set(RTTR_VERSION_INCLUDE_DIR ${RTTR_DIR}/build/${CMAKE_BUILD_TYPE}/src)
set(ASSIMP_INCLUDE_DIR ${ASSIMP_DIR}/assimp)

set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)


# Debug and release specific settings
if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
  add_definitions(-DDEBUG_VERSION)
  add_definitions(-DDBG_MACRO_NO_WARNING)
  add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
else()
  add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
  add_definitions(-DRELEASE_VERSION)
  add_definitions(-DNDEBUG)
endif()
add_definitions(-D_LIBCPP_NO_EXPERIMENTAL_DEPRECATION_WARNING_FILESYSTEM)

# Set the src files for the project
file(GLOB SRC_FILES 
  ${NOBLE_STEED_SRC_DIR}/*.cpp
  ${NOBLE_STEED_SRC_DIR}/core/*.cpp
  ${NOBLE_STEED_SRC_DIR}/graphics/*.cpp
  ${NOBLE_STEED_SRC_DIR}/scene/*.cpp
  ${NOBLE_STEED_SRC_DIR}/io/*.cpp
  ${NOBLE_STEED_SRC_DIR}/math/*.cpp
  ${NOBLE_STEED_SRC_DIR}/hash/*.cpp
  )

# Set project includes dir
include_directories(
  ${NOBLE_STEED_INCLUDE_DIR}
  ${BGFX_INCLUDE_DIR}
  ${BX_INCLUDE_DIR}
  ${BIMG_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
  ${GLM_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
  ${SPDLOG_INCLUDE_DIR}
  ${MEMORY_ALLOCATORS_INCLUDE_DIR}
  ${PYBIND_INCLUDE_DIR}
  ${RTTR_VERSION_INCLUDE_DIR}
  ${RTTR_INCLUDE_DIR}
  ${ASSIMP_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  )
  
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tools)
add_subdirectory(${PYBIND_DIR})
add_subdirectory(${GLFW_DIR})
add_subdirectory(${ASSIMP_DIR})
add_subdirectory(${BULLET_DIR})
add_subdirectory(${SPDLOG_DIR})
add_subdirectory(${MEMORY_ALLOCATORS_DIR})
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
    set(BGFX_LIB_NAME libbgfx-shared-libRelease.dylib)
    set(RTTR_LIB_NAME librttr_core_d.dylib)
    set(BGFX_BIN_DIR "${BGFX_DIR}/.build/osx64_clang/bin")
    set(BGFX_LIB_PATH "${BGFX_BIN_DIR}/${BGFX_LIB_NAME}")
    set(RTTR_LIB_PATH "${RTTR_DIR}/build/${CMAKE_BUILD_TYPE}/lib/${RTTR_LIB_NAME}")
    
    link_directories(/usr/local/lib)
    include_directories(/usr/local/include)    
    add_library(${TARGET_NAME} ${SRC_FILES})
    target_link_libraries(${TARGET_NAME}
        ${BGFX_LIB_PATH}  
        pybind11::embed
        glfw
        ${RTTR_LIB_PATH}
        memory-allocators
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
        LinearMath
        spdlog
        )
else()
    add_definitions(-DPLATFORM_LINUX)
    add_definitions(-DGLFW_EXPOSE_NATIVE_X11)
    add_library(${TARGET_NAME} ${SRC_FILES})
    # Set dependency library paths
    set(BGFX_LIB_NAME libbgfx-shared-libRelease.so)
    if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
      set(RTTR_LIB_NAME librttr_core_d.so)
    else()
      set(RTTR_LIB_NAME librttr_core.so)
    endif()
    set(BGFX_BIN_DIR "${BGFX_DIR}/.build/linux64_gcc/bin")
    set(BGFX_LIB_PATH "${BGFX_BIN_DIR}/${BGFX_LIB_NAME}")
    set(RTTR_LIB_PATH "${RTTR_DIR}/build/${CMAKE_BUILD_TYPE}/lib/${RTTR_LIB_NAME}")
  
    target_link_libraries(${TARGET_NAME}
    ${BGFX_LIB_PATH}
    #pybind11::pybind11
    pybind11::embed
    glfw
    ${RTTR_LIB_PATH}
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
    LinearMath
    memory-allocators
    ${PYTHON_LIBRARIES}
    spdlog
    X11
    GL
    stdc++fs
    pthread
    )
endif()

configure_file(${BGFX_BIN_DIR}/shadercRelease ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools COPYONLY)
configure_file(${BGFX_BIN_DIR}/texturecRelease ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools COPYONLY)
configure_file(${BGFX_BIN_DIR}/texturevRelease ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools COPYONLY)
configure_file(${BGFX_BIN_DIR}/geometrycRelease ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools COPYONLY)
configure_file(${BGFX_BIN_DIR}/geometryvRelease ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools COPYONLY)

# pybind11_add_module(${PYTHON_MODULE_NAME} ${CMAKE_SOURCE_DIR}/src/core/python_bindings.cpp)

# target_link_libraries(${PYTHON_MODULE_NAME} PRIVATE
#   ${TARGET_NAME}
# )

add_subdirectory(samples)