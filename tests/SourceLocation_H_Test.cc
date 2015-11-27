#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(source_location) {
  std::string path("./case/main.cpp");

  clang::Index index(0, 0);

  clang::TranslationUnit tu(index, path, {});
  auto tokens=tu.get_tokens(0, 113);

  auto offsets=(*tokens)[28].offsets;
  
  BOOST_CHECK(offsets.first.line == 6 && offsets.first.index == 3);
  BOOST_CHECK(offsets.second.line == 6 && offsets.second.index == 9);
}
