#ifndef CODECOMPLETERESULTS_H_
#define CODECOMPLETERESULTS_H_
#include <clang-c/Index.h>
#include <map>
#include "CompletionString.h"

namespace clang {
  class CodeCompleteResults {
  public:
    CodeCompleteResults(CXTranslationUnit &tu,
                        const std::string &file_name,
                        const std::map<std::string, std::string>  &buffers,
                        int line_num,
                        int column);
    ~CodeCompleteResults();
    CompletionString get(int index);
    int size();

    CXCodeCompleteResults *results_;
  };
}  // namespace clang
#endif  // CODECOMPLETERESULTS_H_
