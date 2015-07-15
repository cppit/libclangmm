#ifndef SOURCERANGE_H_
#define SOURCERANGE_H_
#include <clang-c/Index.h>
#include "SourceLocation.h"
#include <string>

namespace clang {  
  class RangeData {
  public:
    std::string path;
    unsigned start_offset, end_offset;
  };
  
  class SourceRange {
  public:
    SourceRange(const CXSourceRange& cx_range) : cx_range(cx_range) {}
    SourceRange(SourceLocation &start, SourceLocation &end);
    std::pair<SourceLocation, SourceLocation> get_source_locations();
    static RangeData get_range_data(SourceLocation &start, SourceLocation &end);
    RangeData get_range_data();
    CXSourceRange cx_range;
  };
}  // namespace clang
#endif  // SOURCERANGE_H_
