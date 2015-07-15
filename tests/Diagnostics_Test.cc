#include <boost/test/unit_test.hpp>
#include "clangmm.h"
#include <iostream>
#include <fstream>

using namespace std;

BOOST_AUTO_TEST_CASE(diagnostics_test) {
  std::string path("./case/main_error.cpp");

  clang::Index index(0, 0);

  std::map<std::string, std::string> map_buffers;
  ifstream ifs(path, ifstream::in);
  stringstream ss;
  ss << ifs.rdbuf();

  map_buffers["./case/main_error.cpp"]=ss.str();

  std::vector<std::string> args;  
  clang::TranslationUnit tu(index, path, args, map_buffers);
  
  auto diagnostics=tu.get_diagnostics();
  BOOST_CHECK(diagnostics.size()==1);
  BOOST_CHECK(diagnostics[0].spelling=="use of undeclared identifier 'undeclared_variable'");
  BOOST_CHECK(diagnostics[0].range.path=="./case/main_error.cpp");
  BOOST_CHECK(diagnostics[0].severity==3);
}
