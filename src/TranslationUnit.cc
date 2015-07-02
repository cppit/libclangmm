#include "TranslationUnit.h"
#include "SourceLocation.h"
#include "Tokens.h"

clang::TranslationUnit::
~TranslationUnit() {
  clang_disposeTranslationUnit(tu_);
}

clang::TranslationUnit& clang::TranslationUnit::
operator=(const clang::TranslationUnit &tu) {
  tu_ = tu.tu_;
  return *this;
}

clang::TranslationUnit::
TranslationUnit(Index *index,
                const std::string &filepath,
                const std::vector<std::string> &command_line_args) {
  std::vector<const char*> args;
  for(auto &a: command_line_args) {
    args.push_back(a.c_str());
  }
  tu_ = clang_createTranslationUnitFromSourceFile(index->index_,
                                                  filepath.c_str(),
                                                  args.size(),
                                                  args.data(),
                                                  0,
                                                  NULL);
}

clang::TranslationUnit::
TranslationUnit(Index *index,
                const std::string &filepath) {
  tu_ = clang_createTranslationUnitFromSourceFile(index->index_,
                                                  filepath.c_str(),
                                                  0,
                                                  NULL,
                                                  0,
                                                  NULL);
}

clang::TranslationUnit::
TranslationUnit(clang::Index *index,
                const std::string &filepath,
                const std::vector<std::string> &command_line_args,
                const std::map<std::string, std::string> &buffers,
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
  tu_ =
    clang_parseTranslationUnit(index->index_,
                               filepath.c_str(),
                               args.data(),
                               args.size(),
                               files.data(),
                               files.size(),
                               flags);
}

int clang::TranslationUnit::
ReparseTranslationUnit(const std::string &file_path,
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
  return clang_reparseTranslationUnit(tu_,
                                      files.size(),
                                      files.data(),
                                      flags);
}

unsigned clang::TranslationUnit::DefaultFlags() {
  return CXTranslationUnit_CacheCompletionResults | CXTranslationUnit_PrecompiledPreamble | CXTranslationUnit_Incomplete;
}

std::vector<clang::Diagnostic> clang::TranslationUnit::get_diagnostics() {
  std::vector<clang::Diagnostic> diagnostics;
  for(unsigned c=0;c<clang_getNumDiagnostics(tu_);c++) {
    CXDiagnostic clang_diagnostic=clang_getDiagnostic(tu_, c);
    diagnostics.emplace_back(clang::Diagnostic(*this, clang_diagnostic));
  }
  return diagnostics;
}
