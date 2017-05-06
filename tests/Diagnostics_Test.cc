#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <iostream>
#include <fstream>

using namespace std;

BOOST_AUTO_TEST_CASE(diagnostics_test) {
  std::string path("./case/main_error.cpp");

  clangmm::Index index(0, 0);

  clangmm::TranslationUnit tu(index, path, {});
  
  auto diagnostics=tu.get_diagnostics();
  BOOST_CHECK(diagnostics.size()==1);
  BOOST_CHECK(diagnostics[0].spelling=="use of undeclared identifier 'undeclared_variable'");
  BOOST_CHECK(diagnostics[0].severity==3);
}
