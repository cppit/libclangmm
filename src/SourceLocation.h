#ifndef SOURCELOCATION_H_
#define SOURCELOCATION_H_
#include <clang-c/Index.h>
#include <string>

namespace clang {
  
  class SourceLocation {
  public:
    SourceLocation(CXSourceLocation cx_location) : cx_location(cx_location) {}
    
    SourceLocation(CXTranslationUnit &cx_tu,
                   const std::string &filename,
                   int line_number,
                   int column);

    SourceLocation(CXTranslationUnit &tu,
                   const std::string &filepath,
                   int offset);

    void get_location_info(std::string *path,
                           unsigned *line,
                           unsigned *column,
                           unsigned *offset);

    CXSourceLocation cx_location;
  };

}  // namespace clang
#endif  // SOURCELOCATION_H_
