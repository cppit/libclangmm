#ifndef INDEX_H_
#define INDEX_H_
#include <clang-c/Index.h>

namespace clangmm {
  class Index {
  public:
    Index(int excludeDeclarationsFromPCH, int displayDiagnostics);
    ~Index();
    CXIndex cx_index;
  };
}  // namespace clangmm
#endif  // INDEX_H_
