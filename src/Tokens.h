#ifndef TOKENS_H_
#define TOKENS_H_
#include "TranslationUnit.h"
#include "SourceRange.h"
#include "Token.h"

namespace clang {
  class Tokens {
  public:
    Tokens(TranslationUnit *tu, SourceRange *range);
    ~Tokens();
    std::vector<Token>& tokens();
  protected:
    std::vector<clang::Token> tks;
    CXToken *tokens_;
    unsigned num_tokens_;
  };
}  // namespace clang
#endif  // TOKENS_H_
