#include "CodeCompleteResults.h"
#include "CompletionString.h"

clang::CodeCompleteResults::
CodeCompleteResults(clang::TranslationUnit *tu,
                    const std::string &file_name,
                    const std::map<std::string, std::string>  &buffers,
                    int line_num,
                    int column) {
  std::vector<CXUnsavedFile> files;
  for (auto &buffer : buffers) {
    CXUnsavedFile file;
    file.Filename = buffer.first.c_str();
    file.Contents = buffer.second.c_str();
    file.Length = buffer.second.size();
    files.push_back(file);
  }
  results_ = clang_codeCompleteAt(tu->tu_,
                                  file_name.c_str(),
                                  line_num,
                                  column,
                                  files.data(),
                                  files.size(),
                                  clang_defaultCodeCompleteOptions());
  clang_sortCodeCompletionResults(results_->Results, results_->NumResults);
}

int clang::CodeCompleteResults::
size() {
  return results_->NumResults;
}

clang::CompletionString clang::CodeCompleteResults::
get(int i) {
  if (i > size()) {
    // TODO(zalox) return type
  }
  return CompletionString(results_->Results[i].CompletionString);
}
