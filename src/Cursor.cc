#include "Cursor.h"

const clang::CursorKind clang::Cursor::kind() {
  return (CursorKind) clang_getCursorKind(this->cursor_);
}

clang::Cursor::
Cursor(CXTranslationUnit &tu, clang::SourceLocation *source_location) {
  cursor_ = clang_getCursor(tu, source_location->location_);
}
