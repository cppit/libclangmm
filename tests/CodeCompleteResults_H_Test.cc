#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(code_complete_results) {

  // [ Should be changed with mockery

  std::string path("./case/main.cpp");

  clang::Index index(0, 0);
  clang::TranslationUnit tu(index, path);

  // ReparseTranslationUnit takes a map with filepath as key
  // and buffer as value
  std::map<std::string, std::string> buffers;

  // create buffer
  std::string file;
  file.append("#include <string>\n");
  file.append("int main(int argc, char *argv[]) {\n");
  file.append("std::string str;\n");
  file.append("str.\n");
  file.append("return 0\n");
  file.append("}");

  buffers[path] = file;

  // ]

  auto results=tu.get_code_completions(buffers, 4, 5);

  bool substr_found=false;
  for(int c=0;c<results.size();c++) {
    if(results.get(c).get_chunks()[1].chunk=="substr") {
      substr_found=true;
      break;
    }
  }
  BOOST_CHECK(substr_found);
}
