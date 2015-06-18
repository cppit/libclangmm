cmake_minimum_required (VERSION 2.8.4)
set(project_name clangmm)
project(${project_name})
set(library_installation_path "/usr/local/lib/libclangmm/")

if(APPLE)
  set(Boost_USE_STATIC_LIBS "YES")
  set(CMAKE_MACOSX_RPATH 1)
endif()

enable_testing()
add_subdirectory(src)
add_subdirectory(tests)
