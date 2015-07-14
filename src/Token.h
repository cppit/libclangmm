#ifndef TOKEN_H_
#define TOKEN_H_
#include <clang-c/Index.h>
#include "SourceLocation.h"
#include "SourceRange.h"
#include <string>

namespace clang {
  class SourceLocation;
  class SourceRange;
  
  enum TokenKind {
    Token_Punctuation,
    Token_Keyword,
    Token_Identifier,
    Token_Literal,
    Token_Comment
  };

  class Token {
  public:
    explicit Token(CXTranslationUnit &tu, const CXToken &token);
    const TokenKind kind();
    std::string get_token_spelling();
    SourceLocation get_source_location();
    SourceRange get_source_range();
    std::string type;
    const CXToken& token_;
    CXTranslationUnit &tu;
  };
}  // namespace clang
#endif  // TOKEN_H_
