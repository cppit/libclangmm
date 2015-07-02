#ifndef SOURCERANGE_H_
#define SOURCERANGE_H_
#include "TranslationUnit.h"
#include "Token.h"
#include "Cursor.h"

namespace clang {
  class SourceRange {
  public:
    SourceRange() {}
    SourceRange(TranslationUnit *tu, Token *token);
    SourceRange(SourceLocation *start,
                SourceLocation *end);
    explicit SourceRange(Cursor *cursor);

  private:
    CXSourceRange range_;
    friend Tokens;
    friend SourceLocation;
    friend Diagnostic;
  };
}  // namespace clang
#endif  // SOURCERANGE_H_
