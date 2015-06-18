#include "CompileCommands.h"

clang::CompileCommands::
CompileCommands(const std::string &filename, CompilationDatabase *db) {
  commands_ =
    clang_CompilationDatabase_getCompileCommands(db->db_, filename.c_str());
  if(clang_CompileCommands_getSize(commands_)==0)
    commands_ = clang_CompilationDatabase_getAllCompileCommands(db->db_);
}

clang::CompileCommands::
~CompileCommands() {
  clang_CompileCommands_dispose(commands_);
}

std::vector<clang::CompileCommand> clang::CompileCommands::
get_commands() {
  unsigned N = clang_CompileCommands_getSize(commands_);
  std::vector<clang::CompileCommand> res;
  for (unsigned i = 0; i < N; i++) {
    res.emplace_back(clang::CompileCommand(i, this));
  }
  return res;
}
