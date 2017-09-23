#include "clangmm.h"
#include <string>
#include <cassert>

int main() {
  std::string tests_path=LIBCLANGMM_TESTS_PATH;
  std::string path(tests_path+"/case/main.cpp");

  clangmm::Index index(0, 0);
  clangmm::TranslationUnit tu(index, path, {});

  std::string buffer="#include <string>\n"
                     "int main(int argc, char *argv[]) {\n"
                     "std::string str;\n"
                     "str.\n"
                     "return 0\n"
                     "}";

  tu.reparse(buffer);
  auto results=tu.get_code_completions(buffer, 4, 5);

  bool substr_found=false;
  for(unsigned c=0;c<results.size();c++) {
    if(results.get(c).get_chunks()[1].text=="substr") {
      substr_found=true;
      break;
    }
  }
  assert(substr_found);
}
