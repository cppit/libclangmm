#ifndef TRANSLATIONUNIT_H_
#define TRANSLATIONUNIT_H_
#include <clang-c/Index.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Index.h"

namespace clang {
  class Token;
  class Tokens;
  class SourceLocation;
  class SourceRange;
  class Cursor;
  class CodeCompleteResults;

  class TranslationUnit {
  public:
    TranslationUnit(Index *index,
                    const std::string &filepath,
                    const std::vector<const char*> &command_line_args);
    TranslationUnit(Index *index,
                    const std::string &filepath,
                    const std::vector<const char*> &command_line_args,
                    const std::map<std::string, std::string> &buffers);
    TranslationUnit(Index *index,
                    const std::string &filepath);
    ~TranslationUnit();
    TranslationUnit() { }
    TranslationUnit& operator=(const TranslationUnit &tu);
    int ReparseTranslationUnit(const std::string &file_path,
                               const std::map<std::string, std::string>
                               &buffers);

  private:
    friend Token;
    friend Tokens;
    friend SourceLocation;
    friend SourceRange;
    friend Cursor;
    friend CodeCompleteResults;
    CXTranslationUnit tu_;
  };
}  // namespace clang
#endif  // TRANSLATIONUNIT_H_

