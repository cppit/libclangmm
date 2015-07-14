#ifndef SOURCERANGE_H_
#define SOURCERANGE_H_
#include <clang-c/Index.h>
#include "Token.h"
#include "Cursor.h"
#include "SourceLocation.h"
#include <string>

namespace clang {
  class Token;
  class SourceLocation;
  class Cursor;
  
  class RangeData {
  public:
    std::string path;
    unsigned start_offset, end_offset;
  };
  
  class SourceRange {
  public:
    SourceRange() {}
    SourceRange(Token *token);
    SourceRange(SourceLocation *start,
                SourceLocation *end);
    explicit SourceRange(Cursor *cursor);
    static RangeData get_range_data(SourceLocation &start, SourceLocation &end);
    RangeData get_range_data();
    CXSourceRange range_;
  };
}  // namespace clang
#endif  // SOURCERANGE_H_
