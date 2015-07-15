#include "Token.h"

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
std::string clang::Token::get_token_spelling() {
  CXString s = clang_getTokenSpelling(cx_tu, cx_token);
  return std::string(clang_getCString(s));
}

const clang::TokenKind clang::Token::kind() {
  return (TokenKind) clang_getTokenKind(cx_token);
}

bool clang::Token::has_type() {
  return !clang_Cursor_isNull(clang_getCursorReferenced(cx_cursor));
}

std::string clang::Token::get_type() {
  std::string spelling;
  auto referenced=clang_getCursorReferenced(cx_cursor);
  if(!clang_Cursor_isNull(referenced)) {
    auto type=clang_getCursorType(referenced);
    auto cxstr=clang_getTypeSpelling(type);
    spelling=clang_getCString(cxstr);
    clang_disposeString(cxstr);
    std::string auto_end="";
    //TODO fix const auto
    if((spelling.size()>=4 && spelling.substr(0, 4)=="auto")) {
      auto_end=spelling.substr(4);
      auto type=clang_getCanonicalType(clang_getCursorType(cx_cursor));
      auto cxstr=clang_getTypeSpelling(type);
      spelling=clang_getCString(cxstr);
      clang_disposeString(cxstr);
      if(spelling.find(" ")==std::string::npos)
        spelling+=auto_end;
    }
  }
  return spelling;
}

std::string clang::Token::get_brief_comments() {
  std::string comment_string;
  auto referenced=clang_getCursorReferenced(cx_cursor);
  auto comment=clang_Cursor_getParsedComment(referenced);
  if(clang_Comment_getKind(comment)==CXComment_FullComment) {
    size_t para_c=0;
    for(unsigned c=0;c<clang_Comment_getNumChildren(comment);c++) {
      auto child_comment=clang_Comment_getChild(comment, c);
      if(clang_Comment_getKind(child_comment)==CXComment_Paragraph) {
        para_c++;
        if(para_c>=2)
          break;
        for(unsigned c=0;c<clang_Comment_getNumChildren(child_comment);c++) {
          auto grandchild_comment=clang_Comment_getChild(child_comment, c);
          if(clang_Comment_getKind(grandchild_comment)==CXComment_Text) {
            auto cxstr=clang_TextComment_getText(grandchild_comment);
            comment_string+=clang_getCString(cxstr);
            comment_string+="\n";
            clang_disposeString(cxstr);
            size_t dot_position=comment_string.find(".");
            if(dot_position!=std::string::npos)
              return comment_string.substr(0, dot_position);
          }
          if(clang_Comment_getKind(grandchild_comment)==CXComment_InlineCommand) {
            auto cxstr=clang_InlineCommandComment_getCommandName(grandchild_comment);
            if(comment_string.size()>0)
              comment_string.pop_back();
            if(clang_InlineCommandComment_getNumArgs(grandchild_comment)==0)
              comment_string+=clang_getCString(cxstr);
            clang_disposeString(cxstr);
            for(unsigned arg_c=0;arg_c<clang_InlineCommandComment_getNumArgs(grandchild_comment);arg_c++) {
              auto cxstr=clang_InlineCommandComment_getArgText(grandchild_comment, arg_c);
              if(cxstr.data!=NULL) {
                if(arg_c>0)
                  comment_string+=" ";
                comment_string+=clang_getCString(cxstr);
                clang_disposeString(cxstr);
              }
            }
          }
        }
      }
      /*cout << "  " << clang_Comment_getKind(child_comment) << ", children: " << clang_Comment_getNumChildren(child_comment) << endl;
      auto cxstr=clang_FullComment_getAsHTML(child_comment);
      cout << "  " << clang_getCString(cxstr) << endl;
      clang_disposeString(cxstr);*/
    }
    while(comment_string.size()>0 && (comment_string.back()=='\n' || comment_string.back()==' '))
      comment_string.pop_back();
  }
  
  return comment_string;
}
