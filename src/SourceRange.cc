#include "SourceRange.h"

clangmm::SourceRange::SourceRange(clangmm::SourceLocation &start, clangmm::SourceLocation &end) {
  cx_range = clang_getRange(start.cx_location, end.cx_location);
}

std::pair<clangmm::Offset, clangmm::Offset> clangmm::SourceRange::get_offsets() {
  SourceLocation start(clang_getRangeStart(cx_range)), end(clang_getRangeEnd(cx_range));
  return {start.get_offset(), end.get_offset()};
}