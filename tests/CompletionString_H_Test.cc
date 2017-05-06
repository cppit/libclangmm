#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(completion_chunk) {
  clangmm::CompletionChunk str("(", clangmm::CompletionChunk_LeftBrace);

  BOOST_CHECK(str.chunk == "(");
  BOOST_CHECK(str.kind == clangmm::CompletionChunk_LeftBrace);
}

BOOST_AUTO_TEST_CASE(completion_string) {

  // [ Should be changed with mockery
  
  std::string path("./case/main.cpp");

  clangmm::Index index(0, 0);
  clangmm::TranslationUnit tu(index, path, {});

  std::string buffer="#include <string>\n"
                     "int main(int argc, char *argv[]) {\n"
                     "std::string str;\n"
                     "str.\n"
                     "return 0\n"
                     "}";

  auto results=tu.get_code_completions(buffer, 4, 5);
  // ]

  clangmm::CompletionString str = results.get(0);
  
  BOOST_CHECK(str.get_num_chunks() == 5);
  BOOST_CHECK(str.get_chunks().size() == 5);
}
