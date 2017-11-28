#ifndef TOKENS_H_
#define TOKENS_H_
#include <clang-c/Index.h>
#include "source_range.h"
#include "token.h"
#include <unordered_set>
#include <vector>
#include <memory>

namespace clangmm {
  class Tokens : public std::vector<clangmm::Token> {
    friend class TranslationUnit;
    friend class Diagnostic;
    Tokens(CXTranslationUnit &cx_tu, const SourceRange &range, bool annotate_tokens=true);
  public:
    ~Tokens();
    std::vector<std::pair<clangmm::Offset, clangmm::Offset> > get_similar_token_offsets(Cursor::Kind kind, const std::string &spelling,
                                                                                        const std::unordered_set<std::string> &usrs);
  private:
    CXToken *cx_tokens;
    std::unique_ptr<CXCursor[]> cx_cursors;
    CXTranslationUnit& cx_tu;
  };
}  // namespace clangmm
#endif  // TOKENS_H_
