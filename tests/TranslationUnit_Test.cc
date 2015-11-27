#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>
#include <map>

BOOST_AUTO_TEST_CASE(translation_unit) {
  std::string path("./case/main.cpp");

  clang::Index index(0, 0);
  
  clang::TranslationUnit tu(index, path, {});

  std::string buffer = "int main(int argc, char *argv[]) {\n"
                       "std::cout << \"Hello World!\" << std::endl;\n"
                       "return 0\n"
                       "}\n";

  BOOST_CHECK(tu.ReparseTranslationUnit(buffer) == 0);
}
