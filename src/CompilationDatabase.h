#ifndef COMPILATIONDATABASE_H_
#define COMPILATIONDATABASE_H_
#include <clang-c/CXCompilationDatabase.h>
#include <clang-c/Index.h>
#include <string>

namespace clang {
  class CompileCommands;
  class CompilationDatabase {
  public:
    explicit CompilationDatabase(const std::string &project_path);
    CompilationDatabase();
    ~CompilationDatabase();
  private:
    CXCompilationDatabase db_;
    friend CompileCommands;
  };
}

#endif  // COMPILATIONDATABASE_H_
