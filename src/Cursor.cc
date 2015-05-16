#include "Cursor.h"

const clang::CursorKind clang::Cursor::kind() {
  return (CursorKind) clang_getCursorKind(this->cursor_);
}

clang::Cursor::
Cursor(clang::TranslationUnit *tu, clang::SourceLocation *source_location) {
  cursor_ = clang_getCursor(tu->tu_, source_location->location_);
}
