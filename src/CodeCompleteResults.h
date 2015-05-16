#ifndef CODECOMPLETERESULTS_H_
#define CODECOMPLETERESULTS_H_
#include <clang-c/Index.h>
#include "TranslationUnit.h"

namespace clang {
  class CompletionString;

  class CodeCompleteResults {
  public:
    CodeCompleteResults(TranslationUnit *tu,
                        const std::string &file_name,
                        const std::map<std::string, std::string>  &buffers,
                        int line_num,
                        int column);
    CompletionString get(int index);
    int size();

  private:
    CXCodeCompleteResults *results_;
  };
}  // namespace clang
#endif  // CODECOMPLETERESULTS_H_
