#ifndef SOURCELOCATION_H_
#define SOURCELOCATION_H_
#include "TranslationUnit.h"
#include "Token.h"
#include "Cursor.h"

namespace clang {
  class SourceLocation {
  public:
    SourceLocation(TranslationUnit* tu,
                   const std::string &filename,
                   int line_number,
                   int column);

    SourceLocation(TranslationUnit *tu,
                   Token *token);

    SourceLocation(SourceRange *range, bool start);

    SourceLocation(TranslationUnit *tu,
                   const std::string &filepath,
                   int offset);

    SourceLocation(CXSourceLocation location) {location_=location;}

    explicit SourceLocation(Cursor *cursor);

    void get_location_info(std::string* path,
                           unsigned *line,
                           unsigned *column,
                           unsigned *offset);

  private:
    CXSourceLocation location_;
    friend SourceRange;
    friend Cursor;
  };

}  // namespace clang
#endif  // SOURCELOCATION_H_
