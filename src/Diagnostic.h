#ifndef DIAGNOSTIC_H_
#define DIAGNOSTIC_H_
#include <string>
#include <vector>
#include <clang-c/Index.h>
#include "SourceRange.h"

namespace clang {
  class Diagnostic {
  public:
    Diagnostic(CXTranslationUnit& cx_tu, CXDiagnostic& cx_diagnostic);
    
    static const std::string get_severity_spelling(unsigned severity);

    unsigned severity;
    std::string severity_spelling;
    std::string spelling;
    std::string path;
    std::pair<unsigned, unsigned> offsets;
  };
}

#endif  // DIAGNOSTIC_H_