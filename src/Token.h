#ifndef TOKEN_H_
#define TOKEN_H_
#include <clang-c/Index.h>
#include "SourceLocation.h"
#include "SourceRange.h"
#include "Cursor.h"
#include <string>

namespace clang {
  class Token {
    friend class Tokens;
  public:
    enum Kind {
      Punctuation,
      Keyword,
      Identifier,
      Literal,
      Comment
    };
  private:
    Token(CXTranslationUnit &cx_tu, CXToken &cx_token, CXCursor &cx_cursor): 
      cx_tu(cx_tu), cx_token(cx_token), cx_cursor(cx_cursor), offsets(get_source_range().get_offsets()) {};
  public:
    Kind get_kind();
    std::string get_spelling();
    SourceLocation get_source_location();
    SourceRange get_source_range();
    clang::Cursor get_cursor() {return clang::Cursor(cx_cursor);}

    CXTranslationUnit &cx_tu;
    CXToken& cx_token;
    CXCursor& cx_cursor;
    std::pair<clang::Offset, clang::Offset> offsets;
  };
}  // namespace clang
#endif  // TOKEN_H_
