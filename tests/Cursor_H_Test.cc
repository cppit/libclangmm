#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(cursor) {
  // [ Should be changed with mockery

  std::string path("./case/main.cpp");

  clang::Index index(0, 0);
  clang::TranslationUnit tu(index, path);

  // ]

  auto cursor=tu.get_cursor(path, 103);

  BOOST_CHECK(cursor.get_kind() == clang::CursorKind::ReturnStmt);
}
