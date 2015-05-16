#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>
#include <map>

BOOST_AUTO_TEST_CASE(translation_unit) {
  std::string path("./case/main.cpp");

  clang::Index index(0, 0);
  clang::TranslationUnit tu(&index, path);

  // ReparseTranslationUnit takes a map with filepath as key
  // and buffer as value
  std::map<std::string, std::string> buffers;

  // create buffer
  std::string file = "int main(int argc, char *argv[]) {\n";
  file.append("std::cout << \"Hello World!\" << std::endl;\n");
  file.append("return 0\n");
  file.append("}");

  buffers[path] = file;
  BOOST_CHECK(tu.ReparseTranslationUnit(path, buffers) == 0);
}
