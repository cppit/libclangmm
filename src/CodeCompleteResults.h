#ifndef CODECOMPLETERESULTS_H_
#define CODECOMPLETERESULTS_H_
#include <clang-c/Index.h>
#include <map>
#include "CompletionString.h"

namespace clang {
  class CodeCompleteResults {
    friend class TranslationUnit;
    CodeCompleteResults(CXTranslationUnit &cx_tu, const std::string &file_name,
                        const std::map<std::string, std::string>  &buffers,
                        unsigned line_num, unsigned column);
  public:
    ~CodeCompleteResults();
    CompletionString get(unsigned index);
    unsigned size();

    CXCodeCompleteResults *cx_results;
  };
}  // namespace clang
#endif  // CODECOMPLETERESULTS_H_
