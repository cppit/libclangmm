#ifndef COMPILATIONDATABASE_H_
#define COMPILATIONDATABASE_H_
#include <clang-c/CXCompilationDatabase.h>
#include <clang-c/Index.h>
#include <string>

namespace clangmm {
  class CompilationDatabase {
  public:
    explicit CompilationDatabase(const std::string &project_path);
    ~CompilationDatabase();

    CXCompilationDatabase cx_db;
  };
}

#endif  // COMPILATIONDATABASE_H_
