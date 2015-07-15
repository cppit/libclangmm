#include "Cursor.h"

const clang::CursorKind clang::Cursor::get_kind() {
  return (CursorKind) clang_getCursorKind(this->cx_cursor);
}

clang::Cursor::
Cursor(CXTranslationUnit &cx_tu, clang::SourceLocation &source_location) {
  cx_cursor = clang_getCursor(cx_tu, source_location.cx_location);
}

clang::SourceLocation clang::Cursor::get_source_location() const {
  return SourceLocation(clang_getCursorLocation(cx_cursor));
}

clang::SourceRange clang::Cursor::get_source_range() const {
  return SourceRange(clang_getCursorExtent(cx_cursor));
}

bool clang::Cursor::operator==(const Cursor& rhs) const {
  auto lhs_rd=get_source_range().get_range_data();
  auto rhs_rd=rhs.get_source_range().get_range_data();
  return lhs_rd.path==rhs_rd.path && lhs_rd.start_offset==rhs_rd.start_offset && lhs_rd.end_offset==rhs_rd.end_offset;
}