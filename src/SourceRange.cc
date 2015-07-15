#include "SourceRange.h"

clang::SourceRange::
SourceRange(clang::SourceLocation &start, clang::SourceLocation &end) {
  cx_range = clang_getRange(start.cx_location, end.cx_location);
}

std::pair<clang::SourceLocation, clang::SourceLocation> clang::SourceRange::get_source_locations() {
  return std::pair<SourceLocation, SourceLocation>(clang_getRangeStart(cx_range), clang_getRangeEnd(cx_range));
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
  auto locations=get_source_locations();
  return get_range_data(locations.first, locations.second);
}