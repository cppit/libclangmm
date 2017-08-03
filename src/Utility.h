#ifndef UTILITY_H_
#define UTILITY_H_
#include <clang-c/Index.h>
#include <string>

namespace clangmm {
  std::string to_string(CXString cx_string);
  
  void remove_include_guard(std::string &buffer);
}

#endif  // UTILITY_H_
