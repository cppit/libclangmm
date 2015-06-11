#include "TranslationUnit.h"

clang::TranslationUnit::
~TranslationUnit() {
  //  clang_disposeTranslationUnit(tu_);
}

clang::TranslationUnit& clang::TranslationUnit::
operator=(const clang::TranslationUnit &tu) {
  tu_ = tu.tu_;
  flags = tu.flags;
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
                const std::map<std::string, std::string> &buffers) {
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
                                      flags);
}
