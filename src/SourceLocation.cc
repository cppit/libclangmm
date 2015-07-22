#include "SourceLocation.h"

// // // // // // // //
//  SourceLocation   //
// // // // // // // //
clang::SourceLocation::SourceLocation(CXTranslationUnit &tu, const std::string &filepath, unsigned offset) {
  CXFile file = clang_getFile(tu, filepath.c_str());
  cx_location = clang_getLocationForOffset(tu, file, offset);
}

std::string clang::SourceLocation::get_path() {
  std::string path;
  get_data(&path, NULL, NULL, NULL);
  return path;
}
unsigned clang::SourceLocation::get_offset() {
  unsigned offset;
  get_data(NULL, NULL, NULL, &offset);
  return offset;
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

