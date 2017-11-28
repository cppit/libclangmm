#include "compile_commands.h"

clangmm::CompileCommands::CompileCommands(const std::string &filename, CompilationDatabase &db) {
  if(!filename.empty())
    cx_commands = clang_CompilationDatabase_getCompileCommands(db.cx_db, filename.c_str());
  if(filename.empty() || clang_CompileCommands_getSize(cx_commands)==0)
    cx_commands = clang_CompilationDatabase_getAllCompileCommands(db.cx_db);
}

clangmm::CompileCommands::~CompileCommands() {
  clang_CompileCommands_dispose(cx_commands);
}

std::vector<clangmm::CompileCommand> clangmm::CompileCommands::get_commands() {
  unsigned size = clang_CompileCommands_getSize(cx_commands);
  std::vector<CompileCommand> commands;
  commands.reserve(size);
  for (unsigned i = 0; i < size; i++)
    commands.emplace_back(clang_CompileCommands_getCommand(cx_commands, i));
  return commands;
}
