#include "SourceRange.h"

clang::SourceRange::
SourceRange(clang::TranslationUnit *tu, clang::Token *token) {
  range_ = clang_getTokenExtent(tu->tu_, token->token_);
}

clang::SourceRange::
SourceRange(clang::SourceLocation *start, clang::SourceLocation *end) {
  range_ = clang_getRange(start->location_, end->location_);
}

clang::SourceRange::SourceRange(Cursor *cursor) {
  range_ = clang_getCursorExtent(cursor->cursor_);
}
