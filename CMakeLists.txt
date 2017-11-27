cmake_minimum_required(VERSION 2.8)

project(clangmm)

add_compile_options(-std=c++11 -Wall -Wextra -Wno-unused-parameter)

if(APPLE)
  set(CMAKE_MACOSX_RPATH 1)
endif()

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
  set(NOT_SUB_PROJECT 1)
endif()

add_subdirectory(src)

option(BUILD_TESTING OFF)

if(${NOT_SUB_PROJECT})
  set(BUILD_TESTING ON)
endif()

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
