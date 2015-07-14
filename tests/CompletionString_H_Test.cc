#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(completion_chunk) {
  clang::CompletionChunk str("(", clang::CompletionChunk_LeftBrace);

  BOOST_CHECK(str.chunk == "(");
  BOOST_CHECK(str.kind == clang::CompletionChunk_LeftBrace);
}

BOOST_AUTO_TEST_CASE(completion_string) {

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

  clang::CodeCompleteResults results(tu.tu_, path, buffers, 4, 5);

  // ]

  clang::CompletionString str = results.get(0);
  
  BOOST_CHECK(str.get_num_chunks() == 5);
  BOOST_CHECK(str.get_chunks().size() == 5);
}
