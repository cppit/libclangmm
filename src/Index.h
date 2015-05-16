#ifndef INDEX_H_
#define INDEX_H_

#include <clang-c/Index.h>

namespace clang {
  class TranslationUnit;
  class Index {
  public:
    Index(int excludeDeclarationsFromPCH, int displayDiagnostics);
  private:
    CXIndex index_;
    friend TranslationUnit;
  };
}  // namespace clang
#endif  // INDEX_H_
