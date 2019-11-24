cmake_minimum_required(VERSION 2.8.11)

project(noble_steed)

set(TARGET_NAME noble_steed)

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
set(GLM_INCLUDE_DIR ${GLM_DIR}/glm)
set(BULLET_INCLUDE_DIR ${BULLET_DIR}/src)
set(SPDLOG_INCLUDE_DIR ${SPDLOG_DIR}/include)
set(MEMORY_ALLOCATORS_INCLUDE_DIR ${MEMORY_ALLOCATORS_DIR}/includes)
set(RTTR_INCLUDE_DIR ${RTTR_DIR}/src/rttr)
set(PYTHON_INCLUDE_DIR /usr/include/python3.6)
set(PYBIND_INCLUDE_DIR ${PYBIND_DIR}/include)


#set(ASSIMP_INCLUDE_DIR ${ASSIMP_DIR}/assimp)

# Set dependency library paths
set(BGFX_LIB_PATH ${BGFX_DIR}/.build/linux64_gcc/bin/libbgfx-shared-lib${CMAKE_BUILD_TYPE}.so)

# Debug and release specific settings
if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
  add_definitions(-DDEBUG_VERSION)
else()
  add_definitions(-DRELEASE_VERSION)
endif()

# Set the src files for the project
file(GLOB SRC_FILES 
  "${NOBLE_STEED_SRC_DIR}/*.cpp"
  "${NOBLE_STEED_SRC_DIR}/core/*.cpp"
  "${NOBLE_STEED_SRC_DIR}/graphics/*.cpp"
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

target_link_libraries(${TARGET_NAME}
)

  # link in the libraries for Urho3d
target_link_libraries(${TARGET_NAME}
  ${BGFX_LIB_PATH}
  glfw
  RTTR::Core
  pybind11::pybind11
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
  pthread
)

# Create a shortcut for the compile commands
add_custom_target(move_compile_commands ALL
  COMMAND cmake -E create_symlink ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/build/compile_commands.json
  DEPENDS ${TARGET_NAME}
  )


add_subdirectory(samples)