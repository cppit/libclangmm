#include "SourceLocation.h"

// // // // // // // //
//  SourceLocation   //
// // // // // // // //
clang::SourceLocation::
SourceLocation(CXTranslationUnit &tu,
               const std::string &filename,
               int line_number,
               int line_offset) {
  CXFile file = clang_getFile(tu, filename.c_str());
  cx_location = clang_getLocation(tu, file, line_number, line_offset);
}

clang::SourceLocation::
SourceLocation(CXTranslationUnit &tu, const std::string &filepath, int offset) {
  CXFile file = clang_getFile(tu, filepath.c_str());
  cx_location = clang_getLocationForOffset(tu, file, offset);
}

void clang::SourceLocation::get_data(std::string* path, unsigned *line, unsigned *column, unsigned *offset) {
  if(path==nullptr)
    clang_getExpansionLocation(cx_location, NULL, line, column, offset);
  else {
    CXFile file;
    clang_getExpansionLocation(cx_location, &file, line, column, offset);
    if (file!=NULL) {
      auto cxstr=clang_getFileName(file);
      *path=clang_getCString(cxstr);
      clang_disposeString(cxstr);
    }
  }
}

