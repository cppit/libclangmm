#ifndef SOURCELOCATION_H_
#define SOURCELOCATION_H_
#include <clang-c/Index.h>
#include <string>

namespace clangmm {
  class Offset {
  public:
    bool operator==(const clangmm::Offset &o) {return offset==o.offset;}
    bool operator!=(const clangmm::Offset &o) {return !(*this==o);}
    unsigned line;
    unsigned index; //byte index in line (not char number)
    unsigned offset;
  };
  
  class SourceLocation {
    friend class TranslationUnit;
    SourceLocation(CXTranslationUnit &tu, const std::string &filepath, unsigned offset);
    SourceLocation(CXTranslationUnit &tu, const std::string &filepath, unsigned line, unsigned column);
  public:
    SourceLocation(const CXSourceLocation& cx_location) : cx_location(cx_location) {}

  public:
    std::string get_path();
    clangmm::Offset get_offset();

    CXSourceLocation cx_location;
    
  private:
    void get_data(std::string *path, unsigned *line, unsigned *column, unsigned *offset);
  };

}  // namespace clangmm
#endif  // SOURCELOCATION_H_
