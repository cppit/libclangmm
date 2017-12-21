#include "code_complete_results.h"
#include "completion_string.h"
#include "utility.h"

clangmm::CodeCompleteResults::CodeCompleteResults(CXTranslationUnit &cx_tu, 
                                                const std::string &buffer,
                                                unsigned line_num, unsigned column) {
  CXUnsavedFile files[1];
  auto file_path=to_string(clang_getTranslationUnitSpelling(cx_tu));
  files[0].Filename = file_path.c_str();
  files[0].Contents = buffer.c_str();
  files[0].Length = buffer.size();

  cx_results = clang_codeCompleteAt(cx_tu,
                                  file_path.c_str(),
                                  line_num,
                                  column,
                                  files,
                                  1,
                                  clang_defaultCodeCompleteOptions()|CXCodeComplete_IncludeBriefComments);
  if(cx_results)
    clang_sortCodeCompletionResults(cx_results->Results, cx_results->NumResults);
}

clangmm::CodeCompleteResults::CodeCompleteResults(CodeCompleteResults &&rhs) : cx_results(rhs.cx_results) {
  rhs.cx_results = nullptr;
}

clangmm::CodeCompleteResults &clangmm::CodeCompleteResults::operator=(CodeCompleteResults &&rhs) {
  if(this!=&rhs) {
    if(cx_results)
      clang_disposeCodeCompleteResults(cx_results);
    cx_results=rhs.cx_results;
    rhs.cx_results=nullptr;
  }
  return *this;
}

clangmm::CodeCompleteResults::~CodeCompleteResults() {
  if(cx_results)
    clang_disposeCodeCompleteResults(cx_results);
}

unsigned clangmm::CodeCompleteResults::size() const {
  if(!cx_results)
    return 0;
  return cx_results->NumResults;
}

clangmm::CompletionString clangmm::CodeCompleteResults::get(unsigned i) const {
  return CompletionString(cx_results->Results[i].CompletionString);
}

std::string clangmm::CodeCompleteResults::get_usr() const {
  if(!cx_results)
    return std::string();
  return to_string(clang_codeCompleteGetContainerUSR(cx_results));
}
