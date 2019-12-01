cmake_minimum_required(VERSION 2.8.11)

project(noble_steed)

set(TARGET_NAME noble_steed)
set(PYTHON_MODULE_NAME Noble_Steed)

# Some basica CMAKE values
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

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
set(PYTHON_INCLUDE_DIR /usr/include/python3.6)
set(PYBIND_INCLUDE_DIR ${PYBIND_DIR}/include)
set(RTTR_VERSION_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/deps/rttr/src)
set(ASSIMP_INCLUDE_DIR ${ASSIMP_DIR}/assimp)

# Set dependency library paths
set(BGFX_LIB_PATH ${BGFX_DIR}/.build/linux64_gcc/bin/libbgfx-shared-lib${CMAKE_BUILD_TYPE}.so)

set(PYTHON_APPEND_STRING .cpython-36m-x86_64-linux-gnu.so)

# Debug and release specific settings
if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
  add_definitions(-DDEBUG_VERSION)
  add_definitions(-DDBG_MACRO_NO_WARNING)
  add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
  set(PYTHON_APPEND_STRING d${PYTHON_APPEND_STRING})
else()
add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
add_definitions(-DRELEASE_VERSION)
add_definitions(-DNDEBUG)
endif()

# Set the src files for the project
file(GLOB SRC_FILES 
  "${NOBLE_STEED_SRC_DIR}/*.cpp"
  "${NOBLE_STEED_SRC_DIR}/core/*.cpp"
  "${NOBLE_STEED_SRC_DIR}/graphics/*.cpp"
  "${NOBLE_STEED_SRC_DIR}/scene/*.cpp"
  )

# Set project includes dir
include_directories(
  "${NOBLE_STEED_INCLUDE_DIR}"
  "${BGFX_INCLUDE_DIR}"
  "${BX_INCLUDE_DIR}"
  "${BIMG_INCLUDE_DIR}"
  "${GLFW_INCLUDE_DIR}"
  "${GLM_INCLUDE_DIR}"
  "${BULLET_INCLUDE_DIR}"
  "${SPDLOG_INCLUDE_DIR}"
  "${MEMORY_ALLOCATORS_INCLUDE_DIR}"
  "${RTTR_INCLUDE_DIR}"
  "${PYTHON_INCLUDE_DIR}"
  "${PYBIND_INCLUDE_DIR}"
  "${RTTR_VERSION_INCLUDE_DIR}"
  "${ASSIMP_INCLUDE_DIR}"
  )

add_subdirectory(${RTTR_DIR})
add_subdirectory(${PYBIND_DIR})
add_subdirectory(${GLFW_DIR})
add_subdirectory(${ASSIMP_DIR})
add_subdirectory(${BULLET_DIR})
add_subdirectory(${SPDLOG_DIR})
add_subdirectory(${MEMORY_ALLOCATORS_DIR})


# Add shared libary
add_library(${TARGET_NAME} SHARED ${SRC_FILES})

  # link in the libraries for Urho3d
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
  /usr/lib/python3.6/config-3.6m-x86_64-linux-gnu/libpython3.6.so
  spdlog
  X11
  GL
  stdc++fs
  pthread
)

# Create a shortcut for the compile commands
add_custom_target(move_compile_commands ALL
  COMMAND cmake -E create_symlink ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/build/compile_commands.json
  DEPENDS ${TARGET_NAME}
  )

# pybind11_add_module(${PYTHON_MODULE_NAME} ${CMAKE_SOURCE_DIR}/src/core/python_bindings.cpp)

# target_link_libraries(${PYTHON_MODULE_NAME} PRIVATE
#   ${TARGET_NAME}
# )

# add_custom_target(deploy_files ALL
#   COMMAND mv ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib/${PYTHON_MODULE_NAME}${PYTHON_APPEND_STRING} ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib/${PYTHON_MODULE_NAME}.so
#   DEPENDS ${PYTHON_MODULE_NAME}
#   )

add_subdirectory(samples)