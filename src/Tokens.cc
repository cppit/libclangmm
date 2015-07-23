#include "Tokens.h"
#include "Utility.h"
#include <iostream>
using namespace std;

clang::Tokens::Tokens(CXTranslationUnit &cx_tu, const SourceRange &range): cx_tu(cx_tu) {
  clang_tokenize(cx_tu, range.cx_range, &cx_tokens, &num_tokens);
  cx_cursors.clear();
  cx_cursors.reserve(num_tokens);
  clang_annotateTokens(cx_tu, cx_tokens, num_tokens, cx_cursors.data());
  for (unsigned i = 0; i < num_tokens; i++) {
    emplace_back(Token(cx_tu, cx_tokens[i], cx_cursors[i]));
  }
}

clang::Tokens::~Tokens() {
  clang_disposeTokens(cx_tu, cx_tokens, size());
}

//This works across TranslationUnits! However, to get rename refactoring to work, 
//one have to open all the files that might include a similar token
//Similar tokens defined as tokens with equal referenced cursors. 
std::vector<std::pair<unsigned, unsigned> > clang::Tokens::get_similar_token_offsets(clang::Token& token) {
  std::vector<std::pair<unsigned, unsigned> > offsets;
  auto referenced=token.get_cursor().get_referenced();
  if(referenced) {
    auto referenced_usr=referenced.get_usr();
    for(auto &a_token: *this) {
      auto a_referenced=a_token.get_cursor().get_referenced();
      if(a_referenced && referenced_usr==a_referenced.get_usr() && token.get_spelling()==a_token.get_spelling()) {
        offsets.emplace_back(a_token.offsets);
      }
    }
  }
  return offsets;
}

std::vector<std::pair<std::string, unsigned> > clang::Tokens::get_cxx_methods() {
  std::vector<std::pair<std::string, unsigned> > methods;
  long last_offset=-1;
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
