#ifndef SOURCERANGE_H_
#define SOURCERANGE_H_
#include <clang-c/Index.h>
#include "SourceLocation.h"
#include <string>
#include <utility>

namespace clangmm {  
  class SourceRange {
  public:
    SourceRange(const CXSourceRange& cx_range) : cx_range(cx_range) {}
    SourceRange(SourceLocation &start, SourceLocation &end);
    std::pair<clangmm::Offset, clangmm::Offset> get_offsets();
    CXSourceRange cx_range;
  };
}  // namespace clangmm
#endif  // SOURCERANGE_H_
