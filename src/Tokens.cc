#include "Tokens.h"
#include <iostream>
using namespace std;

clang::Tokens::Tokens(CXTranslationUnit &cx_tu, SourceRange &range): cx_tu(cx_tu) {
  
  clang_tokenize(cx_tu, range.cx_range, &cx_tokens, &num_tokens);
  cx_cursors.clear();
  cx_cursors.reserve(num_tokens);
  clang_annotateTokens(cx_tu, cx_tokens, num_tokens, cx_cursors.data());
  for (int i = 0; i < num_tokens; i++) {
    emplace_back(cx_tu, cx_tokens[i], cx_cursors[i]);
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
  auto referenced_usr=token.get_cursor().get_referenced_usr();
  if(referenced_usr!="") {
    for(auto &a_token: *this) {
      if(referenced_usr==a_token.get_cursor().get_referenced_usr() && token.get_token_spelling()==a_token.get_token_spelling()) {
        auto range_data=a_token.source_range.get_range_data();
        offsets.emplace_back(range_data.start_offset, range_data.end_offset);
      }
    }
  }
  return offsets;
}
