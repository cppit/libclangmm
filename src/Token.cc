#include "Token.h"
#include "Utility.h"

// // // // //
//  Token   //
// // // // //

// returns gets an source location for this token objekt
// based on the translationunit given
clang::SourceLocation clang::Token::get_source_location() {
  return SourceLocation(clang_getTokenLocation(cx_tu, cx_token));
}

// returns a sourcerange that covers this token
clang::SourceRange clang::Token::get_source_range() {
  return SourceRange(clang_getTokenExtent(cx_tu, cx_token));
}
// returns a string description of this tokens kind
std::string clang::Token::get_spelling() {
  return clang::to_string(clang_getTokenSpelling(cx_tu, cx_token));
}

const clang::TokenKind clang::Token::get_kind() {
  return (TokenKind) clang_getTokenKind(cx_token);
}
