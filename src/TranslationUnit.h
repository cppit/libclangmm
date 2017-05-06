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
#include "Cursor.h"

namespace clangmm {
  class TranslationUnit {
  public:
    TranslationUnit(Index &index,
                    const std::string &file_path,
                    const std::vector<std::string> &command_line_args,
                    const std::string &buffer,
                    unsigned flags=DefaultFlags());
    TranslationUnit(Index &index,
                    const std::string &file_path,
                    const std::vector<std::string> &command_line_args,
                    unsigned flags=DefaultFlags());
    ~TranslationUnit();
    
    int ReparseTranslationUnit(const std::string &buffer, unsigned flags=DefaultFlags());
    
    static unsigned DefaultFlags();
    
    void parse(Index &index,
               const std::string &file_path,
               const std::vector<std::string> &command_line_args,
               const std::map<std::string, std::string> &buffers,
               unsigned flags=DefaultFlags());

    clangmm::CodeCompleteResults get_code_completions(const std::string &buffer,
                                                    unsigned line_number, unsigned column);

    std::vector<clangmm::Diagnostic> get_diagnostics();

    std::unique_ptr<Tokens> get_tokens(unsigned start_offset, unsigned end_offset);
    std::unique_ptr<Tokens> get_tokens(unsigned start_line, unsigned start_column,
                                       unsigned end_line, unsigned end_column);

    clangmm::Cursor get_cursor(std::string path, unsigned offset);
    clangmm::Cursor get_cursor(std::string path, unsigned line, unsigned column);

    CXTranslationUnit cx_tu;
  };
}  // namespace clangmm
#endif  // TRANSLATIONUNIT_H_

