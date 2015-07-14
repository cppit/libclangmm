#include "SourceRange.h"

clang::SourceRange::
SourceRange(clang::Token *token) {
  range_ = clang_getTokenExtent(token->tu, token->token_);
}

clang::SourceRange::
SourceRange(clang::SourceLocation *start, clang::SourceLocation *end) {
  range_ = clang_getRange(start->location_, end->location_);
}

clang::SourceRange::SourceRange(Cursor *cursor) {
  range_ = clang_getCursorExtent(cursor->cursor_);
}

clang::RangeData clang::SourceRange::get_range_data(clang::SourceLocation &start, clang::SourceLocation &end) {
  std::string path;
  unsigned start_offset, end_offset;
  start.get_location_info(&path, NULL, NULL, &start_offset);
  end.get_location_info(NULL, NULL, NULL, &end_offset);
  RangeData range_data;
  range_data.path=path;
  range_data.start_offset=start_offset;
  range_data.end_offset=end_offset;
  return range_data;
}

clang::RangeData clang::SourceRange::get_range_data() {
  clang::SourceLocation start(this, true);
  clang::SourceLocation end(this, false);
  return get_range_data(start, end);
}