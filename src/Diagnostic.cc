#include "Diagnostic.h"
#include "SourceLocation.h"
#include "Tokens.h"

clang::Diagnostic::Diagnostic(CXTranslationUnit& tu, CXDiagnostic& clang_diagnostic) {
  severity=clang_getDiagnosticSeverity(clang_diagnostic);
  severity_spelling=get_severity_spelling(severity);
  spelling=clang_getCString(clang_getDiagnosticSpelling(clang_diagnostic));
  clang::SourceLocation location(clang_getDiagnosticLocation(clang_diagnostic));
  
  clang::SourceRange range(&location, &location);
  clang::Tokens tokens(tu, &range);
  if(tokens.size()==1) {
    auto& token=tokens[0];
    clang::SourceRange range=token.get_source_range();
    auto end_location=clang::SourceLocation(&range, false);
    this->range=range.get_range_data(location, end_location);
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