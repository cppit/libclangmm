#ifndef TRANSLATIONUNIT_H_
#define TRANSLATIONUNIT_H_
#include <clang-c/Index.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Index.h"
#include "Diagnostic.h"

namespace clang {
  class TranslationUnit {
  public:
    TranslationUnit(Index *index,
                    const std::string &filepath,
                    const std::vector<std::string> &command_line_args);
    TranslationUnit(Index *index,
                    const std::string &filepath,
                    const std::vector<std::string> &command_line_args,
                    const std::map<std::string, std::string> &buffers,
                    unsigned flags=DefaultFlags());
    TranslationUnit(Index *index,
                    const std::string &filepath);
    ~TranslationUnit();
    TranslationUnit& operator=(const TranslationUnit &tu);
    int ReparseTranslationUnit(const std::string &file_path,
                               const std::map<std::string, std::string>
                               &buffers,
                               unsigned flags=DefaultFlags());
    static unsigned DefaultFlags();
    void update_diagnostics();
    
    std::vector<clang::Diagnostic> diagnostics;

    void parse(Index *index,
               const std::string &filepath,
               const std::vector<std::string> &command_line_args,
               const std::map<std::string, std::string> &buffers,
               unsigned flags=DefaultFlags());
    CXTranslationUnit tu_;
  };
}  // namespace clang
#endif  // TRANSLATIONUNIT_H_

