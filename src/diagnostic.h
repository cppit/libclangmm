#ifndef DIAGNOSTIC_H_
#define DIAGNOSTIC_H_
#include <string>
#include <vector>
#include <clang-c/Index.h>
#include "source_range.h"

namespace clangmm {
  class Diagnostic {
    friend class TranslationUnit;
    Diagnostic(CXTranslationUnit& cx_tu, CXDiagnostic& cx_diagnostic);
  public:
    enum class Severity {
      Ignored = 0,
      Note,
      Warning,
      Error,
      Fatal
    };
    
    class FixIt {
    public:
      FixIt(const std::string &source, const std::pair<clangmm::Offset, clangmm::Offset> &offsets):
        source(source), offsets(offsets) {}
      std::string source;
      std::pair<clangmm::Offset, clangmm::Offset> offsets;
    };

    Severity severity;
    std::string spelling;
    std::string path;
    std::pair<clangmm::Offset, clangmm::Offset> offsets;
    std::vector<FixIt> fix_its;
  };
}

#endif  // DIAGNOSTIC_H_
