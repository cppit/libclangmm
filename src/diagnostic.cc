#include "diagnostic.h"
#include "source_location.h"
#include "tokens.h"
#include "utility.h"

clangmm::Diagnostic::Diagnostic(CXTranslationUnit& cx_tu, CXDiagnostic& cx_diagnostic) {
  severity=static_cast<Severity>(clang_getDiagnosticSeverity(cx_diagnostic));
  spelling=to_string(clang_getDiagnosticSpelling(cx_diagnostic));
  
  SourceLocation location(clang_getDiagnosticLocation(cx_diagnostic));
  path=location.get_path();
  auto offset=location.get_offset();
  auto corrected_location=SourceLocation(cx_tu, path.c_str(), offset.line, offset.index); // to avoid getting macro tokens
  Tokens tokens(cx_tu, SourceRange(corrected_location, corrected_location), false);
  if(tokens.size()==1)
    offsets={offset, tokens.begin()->get_source_range().get_offsets().second};
  
  unsigned num_fix_its=clang_getDiagnosticNumFixIts(cx_diagnostic);
  for(unsigned c=0;c<num_fix_its;c++) {
    CXSourceRange fix_it_range;
    auto source=to_string(clang_getDiagnosticFixIt(cx_diagnostic, c, &fix_it_range));
    fix_its.emplace_back(source, SourceRange(fix_it_range).get_offsets());
  }
}
