cmake_minimum_required (VERSION 2.8.4)
set(project_name clangmm)
project(${project_name})
if(CYGWIN)
  set(library_path "/usr/bin/")
else()
  set(library_path "/usr/local/lib/")
endif()
set(include_path "/usr/local/include/")

if(APPLE)
  set(Boost_USE_STATIC_LIBS "YES")
  set(CMAKE_MACOSX_RPATH 1)
endif()

add_subdirectory(src)

# enable_testing()
# add_subdirectory(tests)
