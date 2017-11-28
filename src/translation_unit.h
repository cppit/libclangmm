#ifndef TRANSLATIONUNIT_H_
#define TRANSLATIONUNIT_H_
#include <clang-c/Index.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "index.h"
#include "diagnostic.h"
#include "tokens.h"
#include "code_complete_results.h"
#include "cursor.h"

namespace clangmm {
  class TranslationUnit {
  public:
    TranslationUnit(Index &index, const std::string &file_path,
                    const std::vector<std::string> &command_line_args,
                    const std::string &buffer,
                    int flags=DefaultFlags());
    TranslationUnit(Index &index, const std::string &file_path,
                    const std::vector<std::string> &command_line_args,
                    int flags=DefaultFlags());
    ~TranslationUnit();
    
    int reparse(const std::string &buffer, int flags=DefaultFlags());
    
    static int DefaultFlags();
    
    void parse(Index &index,
               const std::string &file_path,
               const std::vector<std::string> &command_line_args,
               const std::map<std::string, std::string> &buffers,
               int flags=DefaultFlags());

    CodeCompleteResults get_code_completions(const std::string &buffer,
                                             unsigned line_number, unsigned column);

    std::vector<Diagnostic> get_diagnostics();

    std::unique_ptr<Tokens> get_tokens();
    std::unique_ptr<Tokens> get_tokens(const std::string &path, unsigned start_offset, unsigned end_offset);
    std::unique_ptr<Tokens> get_tokens(unsigned start_offset, unsigned end_offset);
    std::unique_ptr<Tokens> get_tokens(unsigned start_line, unsigned start_column,
                                       unsigned end_line, unsigned end_column);

    Cursor get_cursor(const std::string &path, unsigned offset);
    Cursor get_cursor(const std::string &path, unsigned line, unsigned column);
    Cursor get_cursor(const SourceLocation &location);

    CXTranslationUnit cx_tu;
  };
}  // namespace clangmm
#endif  // TRANSLATIONUNIT_H_

