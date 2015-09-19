#ifndef TOKEN_H_
#define TOKEN_H_
#include <clang-c/Index.h>
#include "SourceLocation.h"
#include "SourceRange.h"
#include "Cursor.h"
#include <string>

namespace clang {
  enum TokenKind {
    Token_Punctuation,
    Token_Keyword,
    Token_Identifier,
    Token_Literal,
    Token_Comment
  };
  
  class Token {
    friend class Tokens;
    Token(CXTranslationUnit &cx_tu, CXToken &cx_token, CXCursor &cx_cursor): 
      cx_tu(cx_tu), cx_token(cx_token), cx_cursor(cx_cursor), offsets(get_source_range().get_offsets()) {};
  public:
    const TokenKind get_kind();
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
