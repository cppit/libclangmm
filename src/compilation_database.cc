#include "compilation_database.h"
#include <exception>

clangmm::CompilationDatabase::CompilationDatabase(const std::string &project_path) {
  cx_db = clang_CompilationDatabase_fromDirectory(project_path.c_str(), &cx_db_error);
}

clangmm::CompilationDatabase::~CompilationDatabase() {
  clang_CompilationDatabase_dispose(cx_db);
}

clangmm::CompilationDatabase::operator bool() const {
  return !static_cast<bool>(cx_db_error);
}
