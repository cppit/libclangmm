#include "Tokens.h"
#include <iostream>
using namespace std;

clang::Tokens::Tokens(clang::TranslationUnit *tu, clang::SourceRange *range): tu(*tu) {
  clang_tokenize(tu->tu_,
                 range->range_,
                 &tokens_,
                 &num_tokens_);
  for (int i = 0; i < num_tokens_; i++) {
    push_back(clang::Token(tokens_[i]));
  }
}

clang::Tokens::~Tokens() {
  clang_disposeTokens(tu.tu_, tokens_, size());
}

void clang::Tokens::update_types(clang::TranslationUnit *tu) {
  clang_cursors.clear();
  clang_cursors.reserve(size());
  clang_annotateTokens(tu->tu_, tokens_, size(), clang_cursors.data());
  
  for(size_t c=0;c<size();c++) {
    auto referenced=clang_getCursorReferenced(clang_cursors[c]);
    if(!clang_Cursor_isNull(referenced)) {
      auto type=clang_getCursorType(referenced);
      auto cxstr=clang_getTypeSpelling(type);
      std::string spelling=clang_getCString(cxstr);
      clang_disposeString(cxstr);
      std::string auto_end="";
      //TODO fix const auto
      if((spelling.size()>=4 && spelling.substr(0, 4)=="auto")) {
        auto_end=spelling.substr(4);
        auto type=clang_getCanonicalType(clang_getCursorType(clang_cursors[c]));
        auto cxstr=clang_getTypeSpelling(type);
        spelling=clang_getCString(cxstr);
        clang_disposeString(cxstr);
        if(spelling.find(" ")==std::string::npos)
          spelling+=auto_end;
      }
      
      (*this)[c].type=spelling;
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
}

std::string clang::Tokens::get_brief_comments(size_t cursor_id) {
  std::string comment_string;
  auto referenced=clang_getCursorReferenced(clang_cursors[cursor_id]);
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