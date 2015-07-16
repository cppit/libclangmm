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

bool clang::Cursor::operator==(const Cursor& rhs) const {
  auto cxstr=clang_getCursorUSR(cx_cursor);
  std::string lhs_str=clang_getCString(cxstr);
  clang_disposeString(cxstr);
  cxstr=clang_getCursorUSR(rhs.cx_cursor);
  std::string rhs_str=clang_getCString(cxstr);
  clang_disposeString(cxstr);
  return lhs_str==rhs_str;
}