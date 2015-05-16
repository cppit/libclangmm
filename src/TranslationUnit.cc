#include "TranslationUnit.h"

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
                const std::vector<const char*> &command_line_args) {
  tu_ = clang_createTranslationUnitFromSourceFile(index->index_,
                                                  filepath.c_str(),
                                                  command_line_args.size(),
                                                  command_line_args.data(),
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
                const std::vector<const char*> &command_line_args,
                const std::map<std::string, std::string> &buffers) {
  std::vector<CXUnsavedFile> files;
  for (auto &buffer : buffers) {
    CXUnsavedFile file;
    file.Filename = buffer.first.c_str();
    file.Contents = buffer.second.c_str();
    file.Length = buffer.second.size();
    files.push_back(file);
  }
  tu_ =
    clang_parseTranslationUnit(index->index_,
                               filepath.c_str(),
                               command_line_args.data(),
                               command_line_args.size(),
                               files.data(),
                               files.size(),
                               clang_defaultEditingTranslationUnitOptions());
}

int clang::TranslationUnit::
ReparseTranslationUnit(const std::string &file_path,
                       const std::map<std::string, std::string>  &buffers) {
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
                                      clang_defaultReparseOptions(tu_));
}
