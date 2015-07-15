#ifndef TRANSLATIONUNIT_H_
#define TRANSLATIONUNIT_H_
#include <clang-c/Index.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Index.h"
#include "Diagnostic.h"
#include "Tokens.h"
#include "CodeCompleteResults.h"

namespace clang {
  class TranslationUnit {
  public:
    TranslationUnit(Index &index,
                    const std::string &filepath,
                    const std::vector<std::string> &command_line_args);
    TranslationUnit(Index &index,
                    const std::string &filepath,
                    const std::vector<std::string> &command_line_args,
                    const std::map<std::string, std::string> &buffers,
                    unsigned flags=DefaultFlags());
    TranslationUnit(Index &index,
                    const std::string &filepath);
    ~TranslationUnit();
    int ReparseTranslationUnit(const std::map<std::string, std::string> &buffers,
                               unsigned flags=DefaultFlags());
    static unsigned DefaultFlags();
    
    void parse(Index &index,
               const std::string &filepath,
               const std::vector<std::string> &command_line_args,
               const std::map<std::string, std::string> &buffers,
               unsigned flags=DefaultFlags());
               
   clang::CodeCompleteResults get_code_completions(const std::map<std::string, std::string> &buffers, int line_number, int column);
   std::vector<clang::Diagnostic> get_diagnostics();
   std::unique_ptr<Tokens> get_tokens(unsigned start_offset, unsigned end_offset);

   CXTranslationUnit cx_tu;
  };
}  // namespace clang
#endif  // TRANSLATIONUNIT_H_

