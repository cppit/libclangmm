#include "Tokens.h"
clang::Tokens::Tokens(clang::TranslationUnit *tu, clang::SourceRange *range) {
  clang_tokenize(tu->tu_,
                 range->range_,
                 &tokens_,
                 &num_tokens_);
  for (int i = 0; i < num_tokens_; i++) {
    tks.push_back(clang::Token(tokens_[i]));
  }
}

clang::Tokens::~Tokens() {
  // instead of using clang_disposeTokens() the implementation
  // of the latter method is just free(token_) the same as
  // delete(tokens_) eliminating the need of tu*
  delete tokens_;
}

std::vector<clang::Token>& clang::Tokens::tokens() {
  return tks;
}
