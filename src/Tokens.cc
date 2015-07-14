#include "Tokens.h"
#include <iostream>
using namespace std;

clang::Tokens::Tokens(CXTranslationUnit &tu, clang::SourceRange *range): tu(tu) {
  clang_tokenize(tu,
                 range->range_,
                 &tokens_,
                 &num_tokens_);
  for (int i = 0; i < num_tokens_; i++) {
    emplace_back(tu, tokens_[i]);
  }
}

clang::Tokens::~Tokens() {
  clang_disposeTokens(tu, tokens_, size());
}

void clang::Tokens::update_types() {
  clang_cursors.clear();
  clang_cursors.reserve(size());
  clang_annotateTokens(tu, tokens_, size(), clang_cursors.data());
  
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

CXCursor clang::Tokens::find_referenced() {
  clang_cursors.clear();
  clang_cursors.reserve(size());
  clang_annotateTokens(tu, tokens_, size(), clang_cursors.data());

  auto kind=clang_getCursorKind(clang_cursors[0]);
  cout << "  " << kind << endl;
  cout << "  Decl: " << clang_isDeclaration(kind) << endl;
  cout << "  Attr: " << clang_isAttribute(kind) << endl;
  cout << "  Ref: " << clang_isReference(kind) << endl;
  cout << "  Expr: " << clang_isExpression(kind) << endl;

  auto referenced=clang_getCursorReferenced(clang_cursors[0]);
  
  kind=clang_getCursorKind(referenced);
  cout << "  " << kind << endl;
  cout << "  Decl: " << clang_isDeclaration(kind) << endl;
  cout << "  Attr: " << clang_isAttribute(kind) << endl;
  cout << "  Ref: " << clang_isReference(kind) << endl;
  cout << "  Expr: " << clang_isExpression(kind) << endl;

  //TODO: To find similar function declarations, these must be equal (if cursor is declaration):
  //TODO: How do we know it is a function? clang_getCursorKind(cursor)==CXCursor_CXXMethod?
  cout << "    " << clang_getCString(clang_getTypeSpelling(clang_getCursorType(referenced))) << endl;
  cout << "    " << clang_getCString(clang_getTypeSpelling(clang_getCursorType(clang_getCursorSemanticParent(referenced)))) << endl;
  cout << "    " << clang_getCString(clang_getCursorSpelling(referenced)) << endl;
  
  return referenced;
}

bool clang::Tokens::equalCursors(CXCursor a, CXCursor b) {
  clang::Cursor cursor_a;
  cursor_a.cursor_=a;
  auto range=clang::SourceRange(&cursor_a);
  auto location=clang::SourceLocation(&range, true);
  std::string path_a;
  unsigned offset_start_a, offset_end_a;
  location.get_location_info(&path_a, NULL, NULL, &offset_start_a);
  location=clang::SourceLocation(&range, false);
  location.get_location_info(NULL, NULL, NULL, &offset_end_a);
  
  clang::Cursor cursor_b;
  cursor_b.cursor_=b;
  range=clang::SourceRange(&cursor_b);
  location=clang::SourceLocation(&range, true);
  std::string path_b;
  unsigned offset_start_b, offset_end_b;
  location.get_location_info(&path_b, NULL, NULL, &offset_start_b);
  location=clang::SourceLocation(&range, false);
  location.get_location_info(NULL, NULL, NULL, &offset_end_b);
  
  return path_a==path_b && offset_start_a==offset_start_b && offset_end_a==offset_end_b;
}

void clang::Tokens::rename(CXCursor& referenced, std::unordered_map<std::string, std::unordered_multimap<unsigned, unsigned> > &ranges) {
  for(size_t c=0;c<size();c++) {
    auto a_referenced=clang_getCursorReferenced(clang_cursors[c]);
    if(equalCursors(a_referenced, referenced)) {
      std::string path;
      unsigned line, column, offset;
      
      clang::Cursor cursor;
      cursor.cursor_=clang_cursors[c];
      auto range=clang::SourceRange(&cursor);
      auto location=clang::SourceLocation(&range, true);
      location.get_location_info(&path, &line, &column, &offset);
      cout << "  start: " << path << ", " << line << ", " << column << endl;
      location=clang::SourceLocation(&range, false);
      location.get_location_info(&path, &line, &column, &offset);
      cout << "  end: " << path << ", " << line << ", " << column << endl;

      
      clang::Cursor referenced_cursor;
      referenced_cursor.cursor_=a_referenced;
      range=clang::SourceRange(&referenced_cursor);
      location=clang::SourceLocation(&range, true);
      location.get_location_info(&path, &line, &column, &offset);
      cout << "    start: " << path << ", " << line << ", " << column << endl;

      location=clang::SourceLocation(&range, false);
      location.get_location_info(&path, &line, &column, &offset);
      cout << "    end: " << path << ", " << line << ", " << column << endl;

      auto type=clang_getCursorType(a_referenced);
      cout << "    " << clang_getCString(clang_getTypeSpelling(type)) << endl;
    }
  }
  //clang_visitChildren(clang_getTranslationUnitCursor(tu.tu_), clang::Tokens::clang_visitor, &referenced);
}

/*CXChildVisitResult clang::Tokens::clang_visitor(CXCursor cursor, CXCursor parent, CXClientData clientData) {
  CXCursor* referenced=(CXCursor*)clientData;
  auto a_referenced=clang_getCursorReferenced(cursor);
  if(clang_equalCursors(a_referenced, *referenced)) {
    cout << "found" << endl;
    clang::Cursor a_cursor;
    a_cursor.cursor_=cursor;
    auto range=clang::SourceRange(&a_cursor);

    auto location=clang::SourceLocation(&range, true);
    std::string path;
    unsigned line, column, offset;
    location.get_location_info(&path, &line, &column, &offset);
    cout << "  start: " << path << ", " << line << ", " << column << endl;

    location=clang::SourceLocation(&range, false);
    location.get_location_info(&path, &line, &column, &offset);
    cout << "  end: " << path << ", " << line << ", " << column << endl;
  }
  
  return CXChildVisit_Recurse;
}*/

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