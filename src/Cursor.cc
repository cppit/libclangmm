#include "Cursor.h"

const clang::CursorKind clang::Cursor::get_kind() {
  return (CursorKind) clang_getCursorKind(this->cx_cursor);
}

clang::SourceLocation clang::Cursor::get_source_location() const {
  return SourceLocation(clang_getCursorLocation(cx_cursor));
}

clang::SourceRange clang::Cursor::get_source_range() const {
  return SourceRange(clang_getCursorExtent(cx_cursor));
}

std::string clang::Cursor::get_usr() const {
  auto cxstr=clang_getCursorUSR(cx_cursor);
  std::string USR=clang_getCString(cxstr);
  clang_disposeString(cxstr);
  return USR;
}

std::string clang::Cursor::get_referenced_usr() const {
  auto referenced=clang_getCursorReferenced(cx_cursor);
  if(!clang_Cursor_isNull(referenced)) {
    return Cursor(referenced).get_usr();
  }
  else
    return "";
}

bool clang::Cursor::operator==(const Cursor& rhs) const {
  return get_usr()==rhs.get_usr();
}