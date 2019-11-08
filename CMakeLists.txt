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

# Set some Dependency values
set(GLFW_BUILD_EXAMPLES, OFF)
set(GLFW_BUILD_TESTS, OFF)
set(ASSIMP_BUILD_TESTS, OFF)

# Dependency directory
set(DEPENDENCY_DIR ${CMAKE_SOURCE_DIR}/deps)

# Set dependency directories
set(GLFW_DIR ${DEPENDENCY_DIR}/glfw)
set(BGFX_DIR ${DEPENDENCY_DIR}/bgfx)
set(BX_DIR ${DEPENDENCY_DIR}/bx)
set(BIMG_DIR ${DEPENDENCY_DIR}/bimg)
set(GLM_DIR ${DEPENDENCY_DIR}/glm)
set(ASSIMP_DIR ${DEPENDENCY_DIR}/assimp)

# Set library source directory
set(NOBLE_STEED_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Set include directories
set(NOBLE_STEED_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(BGFX_INCLUDE_DIR ${BGFX_DIR}/include)
set(GLFW_INCLUDE_DIR ${GLFW_DIR}/include)
set(BX_INCLUDE_DIR ${BX_DIR}/include)
set(BIMG_INCLUDE_DIR ${BIMG_DIR}/include)
set(GLM_INCLUDE_DIR ${GLM_DIR}/glm)
#set(ASSIMP_INCLUDE_DIR ${ASSIMP_DIR}/assimp)

# Set dependency library paths
set(BGFX_LIB_PATH ${BGFX_DIR}/.build/linux64_gcc/bin/libbgfx-shared-lib${CMAKE_BUILD_TYPE}.so)
#set(GLFW_LIB_PATH ${GLFW_DIR}/build/${CMAKE_BUILD_TYPE}/src/libglfw3.a)

# Debug and release specific settings
if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
  add_definitions(-DDEBUG_VERSION)
else()
  add_definitions(-DRELEASE_VERSION)
endif()

# Set the src files for the project
file(GLOB SRC_FILES 
  "${NOBLE_STEED_SRC_DIR}/*.cpp"
  )

# Set project includes dir
include_directories(
  "${NOBLE_STEED_INCLUDE_DIR}"
  "${BGFX_INCLUDE_DIR}"
  "${BX_INCLUDE_DIR}"
  "${BIMG_INCLUDE_DIR}"
  "${GLFW_INCLUDE_DIR}"
  "${GLM_INCLUDE_DIR}"
  )

add_subdirectory(${GLFW_DIR})
add_subdirectory(${ASSIMP_DIR})

# Add shared libary
add_library(${TARGET_NAME} SHARED ${SRC_FILES})

target_link_libraries(${TARGET_NAME}
)

  # link in the libraries for Urho3d
target_link_libraries(${TARGET_NAME}
  pthread
  ${BGFX_LIB_PATH}
  glfw#${GLFW_LIB_PATH}
  X11
  GL
)

# Create a shortcut for the compile commands
add_custom_target(move_compile_commands ALL
  COMMAND cmake -E create_symlink ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/build/compile_commands.json
  DEPENDS ${TARGET_NAME}
  )


add_subdirectory(samples)