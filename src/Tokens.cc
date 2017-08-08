#include "Tokens.h"
#include "Utility.h"
#include <unordered_set>

clangmm::Tokens::Tokens(CXTranslationUnit &cx_tu, const SourceRange &range): cx_tu(cx_tu) {
  clang_tokenize(cx_tu, range.cx_range, &cx_tokens, &num_tokens);
  cx_cursors.resize(num_tokens);
  clang_annotateTokens(cx_tu, cx_tokens, num_tokens, cx_cursors.data());
  for (unsigned i = 0; i < num_tokens; i++) {
    if(cx_cursors[i].kind==CXCursor_DeclRefExpr) { //Temporary fix to a libclang bug
      auto real_cursor=clang_getCursor(cx_tu, clang_getTokenLocation(cx_tu, cx_tokens[i]));
      cx_cursors[i]=real_cursor;
    }
    emplace_back(Token(cx_tu, cx_tokens[i], cx_cursors[i]));
  }
}

clangmm::Tokens::~Tokens() {
  clang_disposeTokens(cx_tu, cx_tokens, size());
}

//This works across TranslationUnits. Similar tokens defined as tokens with equal canonical cursors. 
std::vector<std::pair<clangmm::Offset, clangmm::Offset> > clangmm::Tokens::get_similar_token_offsets(Cursor::Kind kind, const std::string &spelling,
                                                                                                     const std::unordered_set<std::string> &usrs) {
  std::vector<std::pair<Offset, Offset> > offsets;
  for(auto &token: *this) {
    if(token.is_identifier()) {
      auto referenced=token.get_cursor().get_referenced();
      if(referenced && (kind!=Cursor::Kind::CXXMethod || kind==referenced.get_kind()) && spelling==token.get_spelling()) {
        auto referenced_usrs=referenced.get_all_usr_extended();
        for(auto &usr: referenced_usrs) {
          if(usrs.count(usr)) {
            offsets.emplace_back(token.offsets);
            break;
          }
        }
      }
    }
  }
  return offsets;
}
