#include "SourceRange.h"

clang::SourceRange::
SourceRange(clang::SourceLocation &start, clang::SourceLocation &end) {
  cx_range = clang_getRange(start.cx_location, end.cx_location);
}

std::pair<unsigned, unsigned> clang::SourceRange::get_offsets() {
  SourceLocation start(clang_getRangeStart(cx_range)), end(clang_getRangeEnd(cx_range));
  std::pair<unsigned, unsigned> offsets;
  start.get_data(NULL, NULL, NULL, &offsets.first);
  end.get_data(NULL, NULL, NULL, &offsets.second);
  return offsets;
}