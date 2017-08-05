#include "CompileCommand.h"
#include "CompileCommands.h"
#include "Utility.h"

std::vector<std::string> clangmm::CompileCommand::get_arguments() {
  unsigned N = clang_CompileCommand_getNumArgs(cx_command);
  std::vector<std::string> res(N);
  for (unsigned i = 0; i < N; i++) {
    res[i] = to_string(clang_CompileCommand_getArg(cx_command, i));
  }
  return res;
}
