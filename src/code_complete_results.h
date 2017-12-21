#ifndef CODECOMPLETERESULTS_H_
#define CODECOMPLETERESULTS_H_
#include <clang-c/Index.h>
#include <map>
#include <string>
#include "completion_string.h"

namespace clangmm {
  class CodeCompleteResults {
    friend class TranslationUnit;
    
    CodeCompleteResults(CXTranslationUnit &cx_tu, const std::string &buffer,
                        unsigned line_num, unsigned column);
  public:
    CodeCompleteResults(CodeCompleteResults &) = delete;
    CodeCompleteResults(CodeCompleteResults &&rhs);
    CodeCompleteResults &operator=(const CodeCompleteResults &rhs) = delete;
    CodeCompleteResults &operator=(CodeCompleteResults &&rhs);
    ~CodeCompleteResults();
    CompletionString get(unsigned index) const;
    unsigned size() const;
    std::string get_usr() const;

    CXCodeCompleteResults *cx_results;
  };
}  // namespace clangmm
#endif  // CODECOMPLETERESULTS_H_
