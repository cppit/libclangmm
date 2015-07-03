#include "Tokens.h"
#include <iostream>
using namespace std;

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

std::vector<clang::Cursor> clang::Tokens::get_token_cursors(clang::TranslationUnit *tu) {
  std::vector<CXToken> clang_tokens;
  for(auto& token: tks)
    clang_tokens.emplace_back(token.token_);
  std::vector<CXCursor> clang_cursors(clang_tokens.size());
  clang_annotateTokens(tu->tu_, clang_tokens.data(), clang_tokens.size(), &clang_cursors.data()[0]);
  
  std::vector<clang::Cursor> cursors;
  for(auto clang_cursor: clang_cursors) {
    cursors.emplace_back();
    cursors.back().cursor_=clang_cursor;
  }
  
  for(int c=0;c<tks.size();c++) {
    auto referenced=clang_getCursorReferenced(cursors[c].cursor_);
    if(!clang_Cursor_isNull(referenced)) {
      auto type=clang_getCursorType(referenced);
      std::string spelling=clang_getCString(clang_getTypeSpelling(type));
      std::string auto_end="";
      if(spelling.size()>=4 && spelling.substr(0, 4)=="auto") {
        auto_end=spelling.substr(4);
        auto type=clang_getCanonicalType(clang_getCursorType(cursors[c].cursor_));
        spelling=clang_getCString(clang_getTypeSpelling(type));
        if(spelling.find(" ")==std::string::npos)
          spelling+=auto_end;
      }
      tks[c].type=spelling;
      //std::cout << clang_getCString(clang_getTypeSpelling(type)) << ": " << type.kind << endl;
      ////auto cursor=clang_getTypeDeclaration(type); 
      ////tks[c].type=clang_getCString(clang_getCursorSpelling(cursor));
      ////auto type=clang_getCursorType(referenced);
      
    }
    //Testing:
    /*if(tks[c].get_token_spelling(tu)=="text_view") {
      cout << tks[c].get_token_spelling(tu) << endl;
      auto kind=clang_getCursorKind(cursors[c].cursor_);
      cout << "  " << kind << endl;
      cout << "  Decl: " << clang_isDeclaration(kind) << endl;
      cout << "  Attr: " << clang_isAttribute(kind) << endl;
      cout << "  Ref: " << clang_isReference(kind) << endl;
      cout << "  Expr: " << clang_isExpression(kind) << endl;
      auto referenced=clang_getCursorReferenced(cursors[c].cursor_);
      if(!clang_Cursor_isNull(referenced)) {
        cout << "  " << clang_getCursorKind(referenced) << endl;

        clang::Cursor referenced_cursor;
        referenced_cursor.cursor_=referenced;
        auto range=clang::SourceRange(&referenced_cursor);

        auto location=clang::SourceLocation(&range, true);
        std::string path;
        unsigned line, column, offset;
        location.get_location_info(&path, &line, &column, &offset);
        cout << "    start: " << path << ", " << line << ", " << column << endl;

        location=clang::SourceLocation(&range, false);
        location.get_location_info(&path, &line, &column, &offset);
        cout << "    start: " << path << ", " << line << ", " << column << endl;

        auto type=clang_getCursorType(referenced);
        cout << "    " << clang_getCString(clang_getTypeSpelling(type)) << endl;
      }
    }*/
  }
  return cursors;
}