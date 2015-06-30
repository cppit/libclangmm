#include "Diagnostic.h"

const std::string clang::Diagnostic::get_severity_spelling(unsigned severity) {
  switch(severity) {
    case 0:
      return "Ignored";
    case 1:
      return "Note";
    case 2:
      return "Warning";
    case 3:
      return "Error";
    case 4:
      return "Fatal";
    default:
      return "";
  }
}