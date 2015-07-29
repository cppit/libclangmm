#include "TranslationUnit.h"
#include "SourceLocation.h"
#include "Tokens.h"
#include "Utility.h"
#include <fstream>
#include <sstream>

clang::TranslationUnit::
~TranslationUnit() {
  clang_disposeTranslationUnit(cx_tu);
}

clang::TranslationUnit::
TranslationUnit(Index &index,
                const std::string &filepath,
                const std::vector<std::string> &command_line_args) {
  std::map<std::string, std::string> buffers;
  std::ifstream ifs(filepath, std::ifstream::in);
  std::stringstream ss;
  ss << ifs.rdbuf();
  buffers[filepath]=ss.str();
  parse(index, filepath, command_line_args, buffers);
}

clang::TranslationUnit::
TranslationUnit(Index &index,
                const std::string &filepath) {
  std::vector<std::string> command_line_args;
  std::map<std::string, std::string> buffers;
  std::ifstream ifs(filepath, std::ifstream::in);
  std::stringstream ss;
  ss << ifs.rdbuf();
  buffers[filepath]=ss.str();
  parse(index, filepath, command_line_args, buffers);
}

clang::TranslationUnit::
TranslationUnit(clang::Index &index,
                const std::string &filepath,
                const std::vector<std::string> &command_line_args,
                const std::map<std::string, std::string> &buffers,
                unsigned flags) {
  parse(index, filepath, command_line_args, buffers, flags);
}

void clang::TranslationUnit::parse(Index &index,
                                   const std::string &filepath,
                                   const std::vector<std::string> &command_line_args,
                                   const std::map<std::string, std::string>  &buffers,
                                   unsigned flags) {
  std::vector<CXUnsavedFile> files;
  for (auto &buffer : buffers) {
    CXUnsavedFile file;
    file.Filename = buffer.first.c_str();
    file.Contents = buffer.second.c_str();
    file.Length = buffer.second.size();
    files.push_back(file);
  }
  std::vector<const char*> args;
  for(auto &a: command_line_args) {
    args.push_back(a.c_str());
  }
  cx_tu =
   clang_parseTranslationUnit(index.cx_index,
                              filepath.c_str(),
                              args.data(),
                              args.size(),
                              files.data(),
                              files.size(),
                              flags);
}

int clang::TranslationUnit::
ReparseTranslationUnit(const std::map<std::string, std::string>  &buffers,
                       unsigned flags) {
  std::vector<CXUnsavedFile> files;
  for (auto &buffer : buffers) {
    CXUnsavedFile file;
    file.Filename = buffer.first.c_str();
    file.Contents = buffer.second.c_str();
    file.Length = buffer.second.size();
    files.push_back(file);
  }
  return clang_reparseTranslationUnit(cx_tu,
                                      files.size(),
                                      files.data(),
                                      flags);
}

unsigned clang::TranslationUnit::DefaultFlags() {
  return CXTranslationUnit_CacheCompletionResults | CXTranslationUnit_PrecompiledPreamble | CXTranslationUnit_Incomplete | CXTranslationUnit_IncludeBriefCommentsInCodeCompletion;
}

clang::CodeCompleteResults clang::TranslationUnit::get_code_completions(const std::map<std::string, std::string> &buffers, unsigned line_number, unsigned column) {
  auto path=clang::to_string(clang_getTranslationUnitSpelling(cx_tu));

  clang::CodeCompleteResults results(cx_tu, path, buffers, line_number, column);
  return results;
}

std::vector<clang::Diagnostic> clang::TranslationUnit::get_diagnostics() {
  std::vector<clang::Diagnostic> diagnostics;
  for(unsigned c=0;c<clang_getNumDiagnostics(cx_tu);c++) {
    CXDiagnostic clang_diagnostic=clang_getDiagnostic(cx_tu, c);
    diagnostics.emplace_back(clang::Diagnostic(cx_tu, clang_diagnostic));
    clang_disposeDiagnostic(clang_diagnostic);
  }
  return diagnostics;
}

std::unique_ptr<clang::Tokens> clang::TranslationUnit::get_tokens(unsigned start_offset, unsigned end_offset) {
  auto path=clang::to_string(clang_getTranslationUnitSpelling(cx_tu));
  clang::SourceLocation start_location(cx_tu, path, start_offset);
  clang::SourceLocation end_location(cx_tu, path, end_offset);
  clang::SourceRange range(start_location, end_location);
  return std::unique_ptr<Tokens>(new Tokens(cx_tu, range));
}

clang::Cursor clang::TranslationUnit::get_cursor(std::string path, unsigned offset) {
  clang::SourceLocation location(cx_tu, path, offset);
  return Cursor(clang_getCursor(cx_tu, location.cx_location));
}
