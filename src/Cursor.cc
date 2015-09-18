#include "Cursor.h"
#include "Utility.h"

const clang::CursorKind clang::Cursor::get_kind() {
  return (CursorKind) clang_getCursorKind(this->cx_cursor);
}

clang::SourceLocation clang::Cursor::get_source_location() const {
  return SourceLocation(clang_getCursorLocation(cx_cursor));
}

clang::SourceRange clang::Cursor::get_source_range() const {
  return SourceRange(clang_getCursorExtent(cx_cursor));
}

std::string clang::Cursor::get_spelling() const {
  return clang::to_string(clang_getCursorSpelling(cx_cursor));
}

std::string clang::Cursor::get_usr() const {
  return clang::to_string(clang_getCursorUSR(cx_cursor));
}

clang::Cursor clang::Cursor::get_referenced() const {
  return Cursor(clang_getCursorReferenced(cx_cursor));
}

clang::Cursor::operator bool() const {
  return !clang_Cursor_isNull(cx_cursor);
}

bool clang::Cursor::operator==(const Cursor& rhs) const {
  return get_usr()==rhs.get_usr();
}
