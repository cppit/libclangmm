#ifndef TOKEN_H_
#define TOKEN_H_
#include "SourceLocation.h"
#include "SourceRange.h"
#include "TranslationUnit.h"

namespace clang {
  enum TokenKind {
    Token_Punctuation,
    Token_Keyword,
    Token_Identifier,
    Token_Literal,
    Token_Comment
  };

  class Token {
  public:
    const TokenKind kind();
    std::string get_token_spelling(TranslationUnit *tu);
    SourceLocation get_source_location(TranslationUnit *tu);
    SourceRange get_source_range(TranslationUnit *tu);

  private:
    explicit Token(const CXToken &token);
    friend SourceRange;
    friend SourceLocation;
    friend Tokens;
    CXToken token_;
  };
}  // namespace clang
#endif  // TOKEN_H_
