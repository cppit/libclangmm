#include "Diagnostic.h"
#include "SourceLocation.h"
#include "Tokens.h"

clang::Diagnostic::Diagnostic(CXTranslationUnit& cx_tu, CXDiagnostic& cx_diagnostic) {
  severity=clang_getDiagnosticSeverity(cx_diagnostic);
  severity_spelling=get_severity_spelling(severity);
  auto cxstr=clang_getDiagnosticSpelling(cx_diagnostic);
  spelling=clang_getCString(cxstr);
  clang_disposeString(cxstr);
  clang::SourceLocation location(clang_getDiagnosticLocation(cx_diagnostic));
  
  clang::SourceRange range(location, location);
  clang::Tokens tokens(cx_tu, range);
  if(tokens.size()==1) {
    auto& token=tokens[0];
    auto locations=token.source_range.get_source_locations();
    this->range=SourceRange::get_range_data(location, locations.second);
  }
}

const std::string clang::Diagnostic::get_severity_spelling(unsigned severity) {
  switch(severity) {
    case CXDiagnostic_Ignored:
      return "Ignored";
    case CXDiagnostic_Note:
      return "Note";
    case CXDiagnostic_Warning:
      return "Warning";
    case CXDiagnostic_Error:
      return "Error";
    case CXDiagnostic_Fatal:
      return "Fatal";
    default:
      return "";
  }
}