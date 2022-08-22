cmake_minimum_required(VERSION 3.20)

project(Water
  VERSION 0.1.0
  LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set (CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set the build type" FORCE)
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

find_package(GLM REQUIRED)
find_package(Assimp REQUIRED)

add_subdirectory(deps/glad)

file(GLOB SRCS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB INCS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.h")

add_executable(water ${SRCS} ${INCS})
target_link_libraries(water PUBLIC ${GLFW_LIBRARIES} glad ${assimp_LIBRARIES})
target_include_directories(water PUBLIC ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${assimp_INCLUDE_DIRS})
