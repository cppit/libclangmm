#include "Utility.h"

std::string clangmm::to_string(CXString cx_string) {
  std::string string;
  if(cx_string.data!=NULL) {
    string=clang_getCString(cx_string);
    clang_disposeString(cx_string);
  }
  return string;
}