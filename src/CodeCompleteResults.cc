#include "CodeCompleteResults.h"
#include "CompletionString.h"
#include <exception>

clang::CodeCompleteResults::
CodeCompleteResults(CXTranslationUnit &cx_tu,
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
  cx_results = clang_codeCompleteAt(cx_tu,
                                  file_name.c_str(),
                                  line_num,
                                  column,
                                  files.data(),
                                  files.size(),
                                  clang_defaultCodeCompleteOptions()|CXCodeComplete_IncludeBriefComments);
  clang_sortCodeCompletionResults(cx_results->Results, cx_results->NumResults);
}

clang::CodeCompleteResults::~CodeCompleteResults() {
  delete[] cx_results->Results;
  delete cx_results;
}

int clang::CodeCompleteResults::
size() {
  return cx_results->NumResults;
}

clang::CompletionString clang::CodeCompleteResults::
get(int i) {
  if (i >= size()) {
    throw std::invalid_argument("clang::CodeCompleteResults::get(int i): i>=size()");
  }
  return CompletionString(cx_results->Results[i].CompletionString);
}
