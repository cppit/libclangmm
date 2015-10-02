#include "SourceLocation.h"
#include "Utility.h"

// // // // // // // //
//  SourceLocation   //
// // // // // // // //
clang::SourceLocation::SourceLocation(CXTranslationUnit &tu, const std::string &filepath, unsigned offset) {
  CXFile file = clang_getFile(tu, filepath.c_str());
  cx_location = clang_getLocationForOffset(tu, file, offset);
}

clang::SourceLocation::SourceLocation(CXTranslationUnit &tu, const std::string &filepath, unsigned line, unsigned column) {
  CXFile file = clang_getFile(tu, filepath.c_str());
  cx_location = clang_getLocation(tu, file, line, column);
}

std::string clang::SourceLocation::get_path() {
  std::string path;
  get_data(&path, NULL, NULL, NULL);
  return path;
}
clang::Offset clang::SourceLocation::get_offset() {
  unsigned line, index;
  get_data(NULL, &line, &index, NULL);
  return {line, index};
}

void clang::SourceLocation::get_data(std::string* path, unsigned *line, unsigned *column, unsigned *offset) {
  if(path==nullptr)
    clang_getExpansionLocation(cx_location, NULL, line, column, offset);
  else {
    CXFile file;
    clang_getExpansionLocation(cx_location, &file, line, column, offset);
    if (file!=NULL) {
      *path=clang::to_string(clang_getFileName(file));
    }
  }
}

