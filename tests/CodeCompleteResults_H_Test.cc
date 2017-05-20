#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(code_complete_results) {

  // [ Should be changed with mockery

  std::string path("./case/main.cpp");

  clangmm::Index index(0, 0);
  clangmm::TranslationUnit tu(index, path, {});

  std::string buffer="#include <string>\n"
                     "int main(int argc, char *argv[]) {\n"
                     "std::string str;\n"
                     "str.\n"
                     "return 0\n"
                     "}";

  // ]

  tu.ReparseTranslationUnit(buffer);
  auto results=tu.get_code_completions(buffer, 4, 5);

  bool substr_found=false;
  for(unsigned c=0;c<results.size();c++) {
    if(results.get(c).get_chunks()[1].chunk=="substr") {
      substr_found=true;
      break;
    }
  }
  BOOST_CHECK(substr_found);
}
