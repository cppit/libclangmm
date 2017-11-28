#include "utility.h"
#include <regex>

std::string clangmm::to_string(CXString cx_string) {
  std::string string;
  if(cx_string.data!=NULL) {
    string=clang_getCString(cx_string);
    clang_disposeString(cx_string);
  }
  return string;
}

clangmm::String::String(const CXString &cx_string) : cx_string(cx_string) {
  if(cx_string.data!=NULL)
    c_str=clang_getCString(cx_string);
  else
    c_str="";
}

clangmm::String::~String() {
  if(cx_string.data!=NULL)
    clang_disposeString(cx_string);
}

void clangmm::remove_include_guard(std::string &buffer) {
  static std::regex ifndef_regex1("^[ \t]*#[ \t]*ifndef[ \t]+([A-Za-z0-9_]+).*$");
  static std::regex ifndef_regex2("^[ \t]*#[ \t]*if[ \t]+![ \t]*defined[ \t]*\\([ \t]*([A-Za-z0-9_]+).*$");
  static std::regex define_regex("^[ \t]*#[ \t]*define[ \t]+([A-Za-z0-9_]+).*$");
  static std::regex endif_regex("^[ \t]*#[ \t]*endif.*$");
  std::vector<std::pair<size_t, size_t>> ranges;
  bool found_ifndef=false, found_define=false;
  bool line_comment=false, multiline_comment=false;
  size_t start_of_line=0;
  std::string line;
  std::string preprocessor_identifier;
  for(size_t c=0;c<buffer.size();++c) {
    if(!line_comment && !multiline_comment && buffer[c]=='/' && c+1<buffer.size() && (buffer[c+1]=='/' || buffer[c+1]=='*')) {
      if(buffer[c+1]=='/')
        line_comment=true;
      else
        multiline_comment=true;
      ++c;
    }
    else if(multiline_comment && buffer[c]=='*' && c+1<buffer.size() && buffer[c+1]=='/') {
      multiline_comment=false;
      ++c;
    }
    else if(buffer[c]=='\n') {
      bool empty_line=true;
      for(auto &chr: line) {
        if(chr!=' ' && chr!='\t') {
          empty_line=false;
          break;
        }
      }
      
      std::smatch sm;
      if(empty_line) {}
      else if(!found_ifndef && (std::regex_match(line, sm, ifndef_regex1) || std::regex_match(line, sm, ifndef_regex2))) {
        found_ifndef=true;
        ranges.emplace_back(start_of_line, c);
        preprocessor_identifier=sm[1].str();
      }
      else if(found_ifndef && std::regex_match(line, sm, define_regex)) {
        found_define=true;
        ranges.emplace_back(start_of_line, c);
        if(preprocessor_identifier!=sm[1].str())
          return;
        break;
      }
      else
        return;
      
      line_comment=false;
      line.clear();
      if(c+1<buffer.size())
        start_of_line=c+1;
      else
        return;
    }
    else if(!line_comment && !multiline_comment && buffer[c]!='\r')
      line+=buffer[c];
  }
  if(found_ifndef && found_define) {
    size_t last_char_pos=std::string::npos;
    for(size_t c=buffer.size()-1;c!=std::string::npos;--c) {
      if(last_char_pos==std::string::npos) {
        if(buffer[c]!=' ' && buffer[c]!='\t' && buffer[c]!='\r' && buffer[c]!='\n')
          last_char_pos=c;
      }
      else {
        if(buffer[c]=='\n' && c+1<buffer.size()) {
          auto line=buffer.substr(c+1, last_char_pos-c);
          std::smatch sm;
          if(std::regex_match(line, sm, endif_regex)) {
            ranges.emplace_back(c+1, last_char_pos+1);
            for(auto &range: ranges) {
              for(size_t c=range.first;c<range.second;++c) {
                if(buffer[c]!='\r')
                  buffer[c]=' ';
              }
            }
            return;
          }
          return;
        }
      }
    }
  }
}
