#include "clangmm.h"
#include <string>
#include <cassert>
#include <regex>

int main() {
  {
    clangmm::CompletionChunk str("(", clangmm::CompletionChunk_LeftBrace);
  
    assert(str.text == "(");
    assert(str.kind == clangmm::CompletionChunk_LeftBrace);
  }
  
  {
    std::string tests_path=LIBCLANGMM_TESTS_PATH;
    std::string path(tests_path+"/case/main.cpp");
  
    std::vector<std::string> arguments;
    auto clang_version_string=clangmm::to_string(clang_getClangVersion());
    const static std::regex clang_version_regex("^[A-Za-z ]+([0-9.]+).*$");
    std::smatch sm;
    if(std::regex_match(clang_version_string, sm, clang_version_regex)) {
      auto clang_version=sm[1].str();
      arguments.emplace_back("-I/usr/lib/clang/"+clang_version+"/include");
      arguments.emplace_back("-I/usr/lib64/clang/"+clang_version+"/include"); // For Fedora
    }
  
    clangmm::Index index(0, 0);
    clangmm::TranslationUnit tu(index, path, arguments);
  
    std::string buffer="#include <string>\n"
                       "int main(int argc, char *argv[]) {\n"
                       "std::string str;\n"
                       "str.\n"
                       "return 0\n"
                       "}";
  
    tu.reparse(buffer);
    auto results=tu.get_code_completions(buffer, 4, 5);
  
    auto str = results.get(0);
    
    assert(str.get_num_chunks()>0);
    assert(str.get_chunks().size()>0);
  }
}
