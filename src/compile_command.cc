#include "compile_command.h"
#include "compile_commands.h"
#include "utility.h"

std::vector<std::string> clangmm::CompileCommand::get_arguments() {
  unsigned size = clang_CompileCommand_getNumArgs(cx_command);
  std::vector<std::string> arguments;
  for (unsigned i = 0; i < size; i++)
    arguments.emplace_back(to_string(clang_CompileCommand_getArg(cx_command, i)));
  return arguments;
}
