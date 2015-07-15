#ifndef COMPILECOMMAND_H_
#define COMPILECOMMAND_H_
#include "CompilationDatabase.h"
#include <vector>

namespace clang {
  class CompileCommand {
  public:
    CompileCommand(int nth, CompileCommands &commands);
    std::string get_command();
    std::vector<std::string> get_command_as_args();

    CXCompileCommand cx_command;
  };
}
#endif  // COMPILECOMMAND_H_
