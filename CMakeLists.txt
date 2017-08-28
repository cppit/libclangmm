cmake_minimum_required (VERSION 2.8.4)
set(project_name clangmm)
project(${project_name})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Wextra -Wno-unused-parameter")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
find_package(LibClang REQUIRED)

if(APPLE)
  set(Boost_USE_STATIC_LIBS "YES")
  set(CMAKE_MACOSX_RPATH 1)
endif()

add_subdirectory(src)

option(BUILD_TESTING Off)
# To enable tests: cmake -DBUILD_TESTING=1 ..
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
