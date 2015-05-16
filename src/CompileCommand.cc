#include "CompileCommand.h"
#include "CompileCommands.h"
#include <iostream>

clang::CompileCommand::
CompileCommand(int nth, clang::CompileCommands *commands) {
  command_ = clang_CompileCommands_getCommand(commands->commands_, nth);
}

std::string clang::CompileCommand::
get_command() {
  std::string res;
  unsigned N = clang_CompileCommand_getNumArgs(command_);
  for (int i = 0; i < N; i++) {
    res += clang_getCString(clang_CompileCommand_getArg(command_, i));
  }
  return res;
}

std::vector<std::string> clang::CompileCommand::
get_command_as_args() {
  unsigned N = clang_CompileCommand_getNumArgs(command_);
  std::vector<std::string> res(N);
  for (int i = 0; i < N; i++) {
    res[i] = clang_getCString(clang_CompileCommand_getArg(command_, i));
  }
  return res;
}
