#include "Token.h"
#include "Utility.h"

// // // // //
//  Token   //
// // // // //

// returns gets an source location for this token objekt
// based on the translationunit given
clangmm::SourceLocation clangmm::Token::get_source_location() const {
  return SourceLocation(clang_getTokenLocation(cx_tu, cx_token));
}

// returns a sourcerange that covers this token
clangmm::SourceRange clangmm::Token::get_source_range() const {
  return SourceRange(clang_getTokenExtent(cx_tu, cx_token));
}
// returns a string description of this tokens kind
std::string clangmm::Token::get_spelling() const {
  return to_string(clang_getTokenSpelling(cx_tu, cx_token));
}

clangmm::Token::Kind clangmm::Token::get_kind() const {
  return static_cast<Kind>(clang_getTokenKind(cx_token));
}

bool clangmm::Token::is_identifier() const {
  auto token_kind=get_kind();
  auto cursor=get_cursor();
  if(token_kind==clangmm::Token::Kind::Identifier && cursor.is_valid_kind())
    return true;
  else if(token_kind==clangmm::Token::Kind::Keyword && cursor.is_valid_kind()) {
    auto spelling=get_spelling();
    if(spelling=="operator" || (spelling=="bool" && get_cursor().get_spelling()=="operator bool"))
      return true;
  }
  else if(token_kind==clangmm::Token::Kind::Punctuation && cursor.is_valid_kind()) {
    auto referenced=cursor.get_referenced();
    if(referenced) {
      auto referenced_kind=referenced.get_kind();
      if(referenced_kind==Cursor::Kind::FunctionDecl || referenced_kind==Cursor::Kind::CXXMethod || referenced_kind==Cursor::Kind::Constructor)
        return true;
    }
  }
  return false;
}
