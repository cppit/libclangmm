#include "CompilationDatabase.h"
#include <exception>

clang::CompilationDatabase::
CompilationDatabase(const std::string &project_path) {
  CXCompilationDatabase_Error error;
  db_ = clang_CompilationDatabase_fromDirectory(project_path.c_str(), &error);
  if(error) {
    throw std::invalid_argument("clang::CompilationDatabase::CompilationDatabase): CXCompilationDatabase_Error");
  }
}

clang::CompilationDatabase::
~CompilationDatabase() {
  clang_CompilationDatabase_dispose(db_);
}
