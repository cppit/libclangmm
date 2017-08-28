#include "clangmm.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

int main() {
  std::string tests_path=LIBCLANGMM_TESTS_PATH;
  std::string path(tests_path+"/case/main_error.cpp");

  clangmm::Index index(0, 0);

  clangmm::TranslationUnit tu(index, path, {});
  
  auto diagnostics=tu.get_diagnostics();
  assert(diagnostics.size()>0);
  assert(!diagnostics[0].spelling.empty());
  assert(diagnostics[0].severity==3);
}
