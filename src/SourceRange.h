#ifndef SOURCERANGE_H_
#define SOURCERANGE_H_
#include "TranslationUnit.h"
#include "Token.h"
#include "Cursor.h"

namespace clang {
  class SourceRange {
  public:
    SourceRange(TranslationUnit *tu, Token *token);
    SourceRange(SourceLocation *start,
                SourceLocation *end);
    explicit SourceRange(Cursor *cursor);
    ~SourceRange();

  private:
    CXSourceRange range_;
    friend Tokens;
    friend SourceLocation;
  };
}  // namespace clang
#endif  // SOURCERANGE_H_
