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

# Set library source directory
set(NOBLE_STEED_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Set include directories
set(NOBLE_STEED_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(BGFX_INCLUDE_DIR ${BGFX_DIR}/include)
set(GLFW_INCLUDE_DIR ${GLFW_DIR}/include)

# Set dependency library paths
set(BGFX_LIB_PATH ${BGFX_DIR}/.build/linux64_gcc/bin/libbgfx${CMAKE_BUILD_TYPE}.a)
set(GLFW_LIB_PATH ${GLFW_DIR}/build/${CMAKE_BUILD_TYPE}/src/libglfw3.a)

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
  "${GLFW_INCLUDE_DIR}"
  )

# Add shared libary
add_library(${TARGET_NAME} SHARED ${SRC_FILES})

target_link_libraries(${TARGET_NAME}
)

  # link in the libraries for Urho3d
target_link_libraries(${TARGET_NAME}
  pthread
  X11
  #GL
  ${BGFX_LIB_PATH}
  ${GLFW_LIB_PATH}
)

# Create a shortcut for the compile commands
add_custom_target(move_compile_commands ALL
  COMMAND cmake -E create_symlink ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/build/compile_commands.json
  DEPENDS ${TARGET_NAME}
  )


add_subdirectory(samples)