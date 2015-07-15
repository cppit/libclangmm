#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(cursor) {
  // [ Should be changed with mockery

  std::string path("./case/main.cpp");

  clang::Index index(0, 0);
  clang::TranslationUnit tu(index, path);

  // ]

  clang::SourceLocation location(tu.cx_tu, path, 6, 4);
  clang::Cursor cursor(tu.cx_tu, location);

  BOOST_CHECK(cursor.get_kind() == clang::CursorKind::ReturnStmt);
}
