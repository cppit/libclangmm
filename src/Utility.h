#ifndef UTILITY_H_
#define UTILITY_H_
#include <clang-c/Index.h>
#include <string>
#include <memory>

namespace clangmm {
  std::string to_string(CXString cx_string);
  
  class CString {
  public:
    CString(const CXString &cx_string);
    ~CString();
    CXString cx_string;
    const char *data;
  };
  
  void remove_include_guard(std::string &buffer);
}

#endif  // UTILITY_H_
