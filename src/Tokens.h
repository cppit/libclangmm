#ifndef TOKENS_H_
#define TOKENS_H_
#include "TranslationUnit.h"
#include "SourceRange.h"
#include "Token.h"

namespace clang {
  class Tokens : public std::vector<clang::Token> {
  public:
    Tokens(TranslationUnit *tu, SourceRange *range);
    ~Tokens();
    void update_types(clang::TranslationUnit *tu);
    std::string get_brief_comment(size_t cursor_id);
  private:
    CXToken *tokens_;
    unsigned num_tokens_;
    std::vector<CXCursor> clang_cursors;
    TranslationUnit& tu;
  };
}  // namespace clang
#endif  // TOKENS_H_
