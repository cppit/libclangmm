#ifndef COMPILATIONDATABASE_H_
#define COMPILATIONDATABASE_H_
#include <clang-c/CXCompilationDatabase.h>
#include <clang-c/Index.h>
#include <string>

namespace clangmm {
  class CompilationDatabase {
    CXCompilationDatabase_Error cx_db_error;
  public:
    explicit CompilationDatabase(const std::string &project_path);
    ~CompilationDatabase();
    
    operator bool() const;

    CXCompilationDatabase cx_db;
  };
}

#endif  // COMPILATIONDATABASE_H_
