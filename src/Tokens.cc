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

/*CXCursor clang::Tokens::find_referenced() {
  cursors.clear();
  cursors.reserve(size());
  clang_annotateTokens(tu, tokens.data(), size(), cursors.data());

  auto kind=clang_getCursorKind(cursors[0]);
  cout << "  " << kind << endl;
  cout << "  Decl: " << clang_isDeclaration(kind) << endl;
  cout << "  Attr: " << clang_isAttribute(kind) << endl;
  cout << "  Ref: " << clang_isReference(kind) << endl;
  cout << "  Expr: " << clang_isExpression(kind) << endl;

  auto referenced=clang_getCursorReferenced(cursors[0]);
  
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
}*/

void clang::Tokens::rename(CXCursor &referenced) {
  for(size_t c=0;c<size();c++) {
    auto a_referenced=clang_getCursorReferenced(cx_cursors[c]);
    if(Cursor(a_referenced)==Cursor(referenced)) {
      std::string path;
      unsigned line, column, offset;
      
      clang::Cursor cursor(cx_cursors[c]);
      auto range=cursor.get_source_range();
      auto locations=range.get_source_locations();
      locations.first.get_location_info(&path, &line, &column, &offset);
      cout << "  start: " << path << ", " << line << ", " << column << endl;
      locations.second.get_location_info(&path, &line, &column, &offset);
      cout << "  end: " << path << ", " << line << ", " << column << endl;

      
      clang::Cursor referenced_cursor(a_referenced);
      range=referenced_cursor.get_source_range();
      locations=range.get_source_locations();
      locations.first.get_location_info(&path, &line, &column, &offset);
      cout << "    start: " << path << ", " << line << ", " << column << endl;

      locations.second.get_location_info(&path, &line, &column, &offset);
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

    auto location=clang::SourceLocation(range, true);
    std::string path;
    unsigned line, column, offset;
    location.get_location_info(&path, &line, &column, &offset);
    cout << "  start: " << path << ", " << line << ", " << column << endl;

    location=clang::SourceLocation(range, false);
    location.get_location_info(&path, &line, &column, &offset);
    cout << "  end: " << path << ", " << line << ", " << column << endl;
  }
  
  return CXChildVisit_Recurse;
}*/
