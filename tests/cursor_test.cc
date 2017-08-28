#include "clangmm.h"
#include <string>
#include <cassert>

int main() {
  std::string tests_path=LIBCLANGMM_TESTS_PATH;
  std::string path(tests_path+"/case/main.cpp");

  clangmm::Index index(0, 0);
  clangmm::TranslationUnit tu(index, path, {});

  auto cursor=tu.get_cursor(path, 103);

  assert(cursor.get_kind() == clangmm::Cursor::Kind::ReturnStmt);
}
