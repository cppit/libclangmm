#include "Token.h"

// // // // //
//  Token   //
// // // // //

// clang::Token instansiates an token
clang::Token::Token(CXTranslationUnit &tu, const CXToken &token) :
  tu(tu), token_(token) {
}

// returns gets an source location for this token objekt
// based on the translationunit given
clang::SourceLocation clang::Token::get_source_location() {
  return SourceLocation(tu, this);
}

// returns a sourcerange that covers this token
clang::SourceRange clang::Token::get_source_range() {
  return SourceRange(this);
}
// returns a string description of this tokens kind
std::string clang::Token::get_token_spelling() {
  CXString s = clang_getTokenSpelling(tu, token_);
  return std::string(clang_getCString(s));
}

const clang::TokenKind clang::Token::kind() {
  return (TokenKind) clang_getTokenKind(token_);
}
