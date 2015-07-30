#ifndef SOURCELOCATION_H_
#define SOURCELOCATION_H_
#include <clang-c/Index.h>
#include <string>

namespace clang {
  
  class SourceLocation {
    friend class TranslationUnit;
    SourceLocation(CXTranslationUnit &tu, const std::string &filepath, unsigned offset);
  public:
    SourceLocation(const CXSourceLocation& cx_location) : cx_location(cx_location) {}

  public:
    std::string get_path();
    unsigned get_offset();

    CXSourceLocation cx_location;
    
  private:
    void get_data(std::string *path, unsigned *line, unsigned *column, unsigned *offset);
  };

}  // namespace clang
#endif  // SOURCELOCATION_H_
