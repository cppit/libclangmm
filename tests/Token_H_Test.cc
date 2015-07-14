#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <string>

BOOST_AUTO_TEST_CASE(token) {
  std::string path("./case/main.cpp");

  clang::Index index(0, 0);

  clang::TranslationUnit tu(&index, path);
  clang::SourceLocation start(tu.tu_, path, 0);
  clang::SourceLocation end(tu.tu_, path, 7, 1);

  clang::SourceRange range(&start, &end);

  clang::Tokens tokens(tu.tu_, &range);

  BOOST_CHECK(tokens.size() == 32);
  BOOST_CHECK(tokens[1].kind() == clang::TokenKind::Token_Identifier);

  std::string str = tokens[28].get_token_spelling();
  BOOST_CHECK(str == "return");  
}
