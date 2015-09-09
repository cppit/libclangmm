#include "Tokens.h"
#include "Utility.h"
#include <iostream> //TODO: remove
using namespace std; //TODO: remove

clang::Tokens::Tokens(CXTranslationUnit &cx_tu, const SourceRange &range): cx_tu(cx_tu) {
  clang_tokenize(cx_tu, range.cx_range, &cx_tokens, &num_tokens);
  cx_cursors.clear();
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

clang::Tokens::~Tokens() {
  clang_disposeTokens(cx_tu, cx_tokens, size());
}

//This works across TranslationUnits! However, to get rename refactoring to work, 
//one have to open all the files that might include a similar token
//Similar tokens defined as tokens with equal referenced cursors. 
std::vector<std::pair<clang::Offset, clang::Offset> > clang::Tokens::get_similar_token_offsets(const std::string &usr) {
  std::vector<std::pair<clang::Offset, clang::Offset> > offsets;
  for(auto &token: *this) {
    if(token.get_kind()==clang::Token_Identifier) {
      auto referenced=token.get_cursor().get_referenced();
      if(referenced && usr==referenced.get_usr()) {
        offsets.emplace_back(token.offsets);
      }
    }
  }
  return offsets;
}

std::vector<std::pair<std::string, clang::Offset> > clang::Tokens::get_cxx_methods() {
  std::vector<std::pair<std::string, clang::Offset> > methods;
  clang::Offset last_offset={(unsigned)-1,(unsigned) -1};
  for(auto &token: *this) {
    if(token.get_kind()==clang::Token_Identifier) {
      auto cursor=token.get_cursor();
      auto kind=cursor.get_kind();
      if(kind==clang::CursorKind::CXXMethod || kind==clang::CursorKind::Constructor || kind==clang::CursorKind::Destructor) {
        auto offset=cursor.get_source_location().get_offset();
        if(offset!=last_offset) {
          std::string method;
          if(kind==clang::CursorKind::CXXMethod) {
            auto type=clang_getResultType(clang_getCursorType(cursor.cx_cursor));
            method+=clang::to_string(clang_getTypeSpelling(type));
            auto pos=method.find(" ");
            if(pos!=std::string::npos)
              method.erase(pos, 1);
            method+=" ";
          }
          
          clang::Cursor parent(clang_getCursorSemanticParent(cursor.cx_cursor));
          method+=clang::to_string(clang_getCursorDisplayName(parent.cx_cursor));
          
          method+="::";
          
          method+=clang::to_string(clang_getCursorDisplayName(cursor.cx_cursor));
          methods.emplace_back(method, offset);
        }
        last_offset=offset;
      }
    }
  }
  return methods;
}
