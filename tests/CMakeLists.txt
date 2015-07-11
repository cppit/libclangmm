set(project_tests ${project_name}_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_HOME_DIRECTORY}/cmake/Modules/")

ADD_DEFINITIONS(-DBOOST_TEST_STATIC_LINK) 

message("Searcing for libclang")
#LIBCLANG_FOUND System has libclang.
#LIBCLANG_INCLUDE_DIRS The libclang include directories.
#LIBCLANG_LIBRARIES  The libraries needed to use libclang.
#LIBCLANG_LIBRARY_DIR The path to the directory containing libclang.
#LIBCLANG_KNOWN_LLVM_VERSIONS  Known LLVM release numbers.
find_package(LibClang REQUIRED)
find_package(Boost 1.55 COMPONENTS unit_test_framework system filesystem REQUIRED)

add_executable(${project_tests}
  Entry.cc
  TranslationUnit_Test.cc
  CompletionString_H_Test.cc
  CodeCompleteResults_H_Test.cc
  Cursor_H_Test.cc
  Token_H_Test.cc
  SourceLocation_H_Test.cc
  Diagnostics_Test.cc
  )

include_directories(${LIBCLANG_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${project_tests} ${LIBCLANG_LIBRARIES} ${Boost_LIBRARIES} clangmm)

set(tests
  ${project_tests}
  )

add_test(${tests} ${tests})