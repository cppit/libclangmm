#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(code_complete_results) {

  // [ Should be changed with mockery

  std::string path("./case/main.cpp");

  clang::Index index(0, 0);
  clang::TranslationUnit tu(&index, path);

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

  clang::CodeCompleteResults results(&tu, path, buffers, 4, 5);

  BOOST_CHECK(results.size() == 105);
  BOOST_CHECK(results.get(0).get_num_chunks() == 5);
}
