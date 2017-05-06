#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(cursor) {
  // [ Should be changed with mockery

  std::string path("./case/main.cpp");

  clangmm::Index index(0, 0);
  clangmm::TranslationUnit tu(index, path, {});

  // ]

  auto cursor=tu.get_cursor(path, 103);

  BOOST_CHECK(cursor.get_kind() == clangmm::Cursor::Kind::ReturnStmt);
}
