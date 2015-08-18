#include "Diagnostic.h"
#include "SourceLocation.h"
#include "Tokens.h"
#include "Utility.h"

clang::Diagnostic::Diagnostic(CXTranslationUnit& cx_tu, CXDiagnostic& cx_diagnostic) {
  severity=clang_getDiagnosticSeverity(cx_diagnostic);
  severity_spelling=get_severity_spelling(severity);
  spelling=clang::to_string(clang_getDiagnosticSpelling(cx_diagnostic));
  
  clang::SourceLocation start_location(clang_getDiagnosticLocation(cx_diagnostic));
  path=start_location.get_path();
  auto start_offset=start_location.get_offset();
  clang::Tokens tokens(cx_tu, SourceRange(start_location, start_location));
  if(tokens.size()==1)
    offsets={start_offset, tokens.begin()->offsets.second};
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