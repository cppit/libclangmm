#include "clangmm.h"
#include <string>
#include <cassert>

int main() {
  std::string tests_path=LIBCLANGMM_TESTS_PATH;
  std::string path(tests_path+"/case/main.cpp");

  clangmm::Index index(0, 0);

  clangmm::TranslationUnit tu(index, path, {});
  auto tokens=tu.get_tokens(0, 113);

  auto offsets=(*tokens)[28].get_source_range().get_offsets();
  
  assert(offsets.first.line == 6 && offsets.first.index == 3);
  assert(offsets.second.line == 6 && offsets.second.index == 9);
}
