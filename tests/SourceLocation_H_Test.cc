#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(source_location) {
  std::string path("./case/main.cpp");

  clang::Index index(0, 0);

  clang::TranslationUnit tu(&index, path);
  clang::SourceLocation start(tu.tu_, path, 0);
  clang::SourceLocation end(tu.tu_, path, 7, 1);
  clang::SourceRange range(&start, &end);
  clang::Tokens tokens(tu.tu_, &range);

  clang::SourceRange token_range  = tokens[28].get_source_range();

  unsigned token_start_line, token_start_column, token_start_offset,
    token_end_line, token_end_column, token_end_offset;
  std::string token_start_path, token_end_path;

  clang::SourceLocation token_start(&token_range, true);
  clang::SourceLocation token_end(&token_range, false);

  token_start.get_location_info(&token_start_path,
                                &token_start_line,
                                &token_start_column,
                                &token_start_offset);

  token_end.get_location_info(&token_end_path,
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
