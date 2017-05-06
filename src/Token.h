#ifndef TOKEN_H_
#define TOKEN_H_
#include <clang-c/Index.h>
#include "SourceLocation.h"
#include "SourceRange.h"
#include "Cursor.h"
#include <string>

namespace clangmm {
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
    Kind get_kind() const;
    std::string get_spelling() const;
    SourceLocation get_source_location() const;
    SourceRange get_source_range() const;
    clangmm::Cursor get_cursor() const {return clangmm::Cursor(cx_cursor);}

    bool is_identifier() const;

    CXTranslationUnit &cx_tu;
    CXToken& cx_token;
    CXCursor& cx_cursor;
    std::pair<clangmm::Offset, clangmm::Offset> offsets;
  };
}  // namespace clangmm
#endif  // TOKEN_H_
