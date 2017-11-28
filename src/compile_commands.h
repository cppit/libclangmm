#ifndef COMPILECOMMANDS_H_
#define COMPILECOMMANDS_H_
#include "compilation_database.h"
#include "compile_command.h"
#include <clang-c/CXCompilationDatabase.h>
#include <string>
#include <vector>

namespace clangmm {
  class CompileCommands {
  public:
    CompileCommands(const std::string &filename, CompilationDatabase &db);
    std::vector<CompileCommand> get_commands();
    ~CompileCommands();

    CXCompileCommands cx_commands;
  };
}
#endif  // COMPILECOMMANDS_H_
