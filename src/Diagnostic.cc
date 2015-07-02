#include "Diagnostic.h"
#include "SourceLocation.h"
#include "Tokens.h"

clang::Diagnostic::Diagnostic(clang::TranslationUnit& tu, CXDiagnostic clang_diagnostic) {
  severity=clang_getDiagnosticSeverity(clang_diagnostic);
  severity_spelling=get_severity_spelling(severity);
  spelling=clang_getCString(clang_getDiagnosticSpelling(clang_diagnostic));
  clang::SourceLocation location(clang_getDiagnosticLocation(clang_diagnostic));
  std::string tmp_path;
  unsigned line, column, offset;
  location.get_location_info(&tmp_path, &line, &column, &offset);
  path=tmp_path;
  start_location.line=line;
  start_location.column=column;
  start_location.offset=offset;
  
  clang::SourceRange range(&location, &location);
  clang::Tokens tokens(&tu, &range);
  if(tokens.tokens().size()==1) {
    auto& token=tokens.tokens()[0];
    clang::SourceRange range=token.get_source_range(&tu);
    clang::SourceLocation location(&range, false);
    location.get_location_info(NULL, &line, &column, &offset);
    end_location.line=line;
    end_location.column=column;
    end_location.offset=offset;
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