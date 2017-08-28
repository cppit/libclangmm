#include "clangmm.h"
#include <string>
#include <cassert>

int main() {
  std::string tests_path=LIBCLANGMM_TESTS_PATH;
  std::string path(tests_path+"/case/main.cpp");

  clangmm::Index index(0, 0);

  clangmm::TranslationUnit tu(index, path, {});
  
  auto tokens=tu.get_tokens(0, 113);

  assert(tokens->size() == 32);
  assert((*tokens)[1].get_kind() == clangmm::Token::Kind::Identifier);

  std::string str = (*tokens)[28].get_spelling();
  assert(str == "return");  
}
