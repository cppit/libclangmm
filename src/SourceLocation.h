#ifndef SOURCELOCATION_H_
#define SOURCELOCATION_H_
#include <clang-c/Index.h>
#include "Token.h"
#include "Cursor.h"
#include <string>

namespace clang {
  class Token;
  class SourceRange;
  class Cursor;
  
  class SourceLocation {
  public:
    SourceLocation(CXTranslationUnit &tu,
                   const std::string &filename,
                   int line_number,
                   int column);

    SourceLocation(CXTranslationUnit &tu,
                   Token *token);

    SourceLocation(SourceRange *range, bool start);

    SourceLocation(CXTranslationUnit &tu,
                   const std::string &filepath,
                   int offset);

    SourceLocation(CXSourceLocation location) {location_=location;}

    explicit SourceLocation(Cursor *cursor);

    void get_location_info(std::string* path,
                           unsigned *line,
                           unsigned *column,
                           unsigned *offset);

    CXSourceLocation location_;
  };

}  // namespace clang
#endif  // SOURCELOCATION_H_
