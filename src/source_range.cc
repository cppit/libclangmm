#include "source_range.h"

clangmm::SourceRange::SourceRange(const clangmm::SourceLocation &start, const clangmm::SourceLocation &end) {
  cx_range = clang_getRange(start.cx_location, end.cx_location);
}

clangmm::SourceLocation clangmm::SourceRange::get_start() const {
  return SourceLocation(clang_getRangeStart(cx_range));
}

clangmm::SourceLocation clangmm::SourceRange::get_end() const {
  return SourceLocation(clang_getRangeEnd(cx_range));
}


std::pair<clangmm::Offset, clangmm::Offset> clangmm::SourceRange::get_offsets() const {
  return {get_start().get_offset(), get_end().get_offset()};
}