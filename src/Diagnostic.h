#ifndef DIAGNOSTIC_H_
#define DIAGNOSTIC_H_
#include "TranslationUnit.h"

namespace clang {  
  class Diagnostic {
  public:
    class LocationData {
    public:
      unsigned line, column, offset;
    };

    unsigned severity;
    std::string spelling;
    std::string path;
    LocationData start_location, end_location;
  };
}

#endif  // DIAGNOSTIC_H_