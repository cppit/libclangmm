#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(source_location) {
  std::string path("./case/main.cpp");

  clang::Index index(0, 0);

  clang::TranslationUnit tu(index, path);
  auto tokens=tu.get_tokens(0, 113);

  clang::SourceRange token_range  = (*tokens)[28].source_range;

  unsigned token_start_line, token_start_column, token_start_offset,
    token_end_line, token_end_column, token_end_offset;
  std::string token_start_path, token_end_path;

  auto locations=token_range.get_source_locations();

  locations.first.get_location_info(&token_start_path,
                                    &token_start_line,
                                    &token_start_column,
                                    &token_start_offset);

  locations.second.get_location_info(&token_end_path,
                                     &token_end_line,
                                     &token_end_column,
                                     &token_end_offset);

  BOOST_CHECK(token_start_path == path);
  BOOST_CHECK(token_start_line == 6);
  BOOST_CHECK(token_start_column == 3);
  BOOST_CHECK(token_start_offset == 103);
  BOOST_CHECK(token_end_path == path);
  BOOST_CHECK(token_end_line == 6);
  BOOST_CHECK(token_end_column == 9);
  BOOST_CHECK(token_end_offset == 109);
}
