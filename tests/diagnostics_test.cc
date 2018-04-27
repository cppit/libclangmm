#include "clangmm.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <regex>

using namespace std;

int main() {
  std::string tests_path=LIBCLANGMM_TESTS_PATH;
  std::string path(tests_path+"/case/main_error.cpp");

  clangmm::Index index(0, 0);

  std::vector<std::string> arguments;
  auto clang_version_string=clangmm::to_string(clang_getClangVersion());
  const static std::regex clang_version_regex("^[A-Za-z ]+([0-9.]+).*$");
  std::smatch sm;
  if(std::regex_match(clang_version_string, sm, clang_version_regex)) {
    auto clang_version=sm[1].str();
    arguments.emplace_back("-I/usr/lib/clang/"+clang_version+"/include");
    arguments.emplace_back("-I/usr/lib64/clang/"+clang_version+"/include"); // For Fedora
  }

  clangmm::TranslationUnit tu(index, path, arguments);
  
  auto diagnostics=tu.get_diagnostics();
  assert(diagnostics.size()>0);
  assert(!diagnostics[0].spelling.empty());
  assert(diagnostics[0].severity==clangmm::Diagnostic::Severity::Error);
}
