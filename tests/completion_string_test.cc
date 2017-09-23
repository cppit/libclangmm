#include "clangmm.h"
#include <string>
#include <cassert>

int main() {
  {
    clangmm::CompletionChunk str("(", clangmm::CompletionChunk_LeftBrace);
  
    assert(str.text == "(");
    assert(str.kind == clangmm::CompletionChunk_LeftBrace);
  }
  
  {
    std::string tests_path=LIBCLANGMM_TESTS_PATH;
    std::string path(tests_path+"/case/main.cpp");
  
    clangmm::Index index(0, 0);
    clangmm::TranslationUnit tu(index, path, {});
  
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
