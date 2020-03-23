cmake_minimum_required(VERSION 3.16.0)

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
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

# Set dependency library paths
set(BGFX_LIB_NAME libbgfx-shared-libRelease.dylib)
set(RTTR_LIB_NAME librttr_core_d.dylib)
set(BGFX_LIB_PATH "${BGFX_DIR}/.build/osx64_clang/bin/${BGFX_LIB_NAME}")
set(RTTR_LIB_PATH "${RTTR_DIR}/build/${CMAKE_BUILD_TYPE}/lib/${RTTR_LIB_NAME}")

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

# Set the src files for the project
file(GLOB SRC_FILES 
  ${NOBLE_STEED_SRC_DIR}/*.cpp
  ${NOBLE_STEED_SRC_DIR}/core/*.cpp
  ${NOBLE_STEED_SRC_DIR}/graphics/*.cpp
  ${NOBLE_STEED_SRC_DIR}/scene/*.cpp
  ${NOBLE_STEED_SRC_DIR}/serialization/*.cpp
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
  
add_subdirectory(${PYBIND_DIR})
add_subdirectory(${GLFW_DIR})
add_subdirectory(${ASSIMP_DIR})
add_subdirectory(${BULLET_DIR})
add_subdirectory(${SPDLOG_DIR})
add_subdirectory(${MEMORY_ALLOCATORS_DIR})

if (WIN32)
# Windows stuff here
elseif(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
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
add_library(${TARGET_NAME} SHARED ${SRC_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE
  ${BGFX_LIB_PATH}
  #pybind11::pybind11
  pybind11::embed
  glfw
  RTTR::Core
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
  memory_allocators
  ${PYTHON_LIBRARIES}
  spdlog
  X11
  GL
  stdc++fs
  pthread
)
endif()

# pybind11_add_module(${PYTHON_MODULE_NAME} ${CMAKE_SOURCE_DIR}/src/core/python_bindings.cpp)

# target_link_libraries(${PYTHON_MODULE_NAME} PRIVATE
#   ${TARGET_NAME}
# )

# add_custom_target(deploy_files ALL
#   COMMAND mv ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib/${PYTHON_MODULE_NAME}${PYTHON_APPEND_STRING} ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib/${PYTHON_MODULE_NAME}.so
#   DEPENDS ${PYTHON_MODULE_NAME}
#   )

add_subdirectory(samples)