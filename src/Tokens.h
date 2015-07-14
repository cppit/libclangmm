#ifndef TOKENS_H_
#define TOKENS_H_
#include <clang-c/Index.h>
#include "SourceRange.h"
#include "Token.h"
#include <unordered_map>
#include <vector>

namespace clang {
  class Tokens : public std::vector<clang::Token> {
  public:
    Tokens(CXTranslationUnit &tu, SourceRange *range);
    ~Tokens();
    void update_types();
    std::string get_brief_comments(size_t cursor_id);
    CXCursor find_referenced();
    bool equalCursors(CXCursor a, CXCursor b);
    void rename(CXCursor &referenced, std::unordered_map<std::string, std::unordered_multimap<unsigned, unsigned> > &ranges);
    
  private:
    CXToken *tokens_;
    unsigned num_tokens_;
    std::vector<CXCursor> clang_cursors;
    CXTranslationUnit& tu;
    
    static CXChildVisitResult clang_visitor(CXCursor cursor, CXCursor parent, CXClientData clientData);
  };
}  // namespace clang
#endif  // TOKENS_H_
