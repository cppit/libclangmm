#include "clangmm.h"
#include <string>
#include <map>
#include <cassert>

int main() {
  std::string tests_path=LIBCLANGMM_TESTS_PATH;
  std::string path(tests_path+"/case/main.cpp");

  clangmm::Index index(0, 0);
  
  clangmm::TranslationUnit tu(index, path, {});

  std::string buffer = "int main(int argc, char *argv[]) {\n"
                       "std::cout << \"Hello World!\" << std::endl;\n"
                       "return 0\n"
                       "}\n";

  assert(tu.reparse(buffer) == 0);
}
