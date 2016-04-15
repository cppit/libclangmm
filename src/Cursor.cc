#include "Cursor.h"
#include "Utility.h"
#include <algorithm>

clang::CursorKind clang::Cursor::get_kind() {
  return static_cast<CursorKind>(clang_getCursorKind(this->cx_cursor));
}

clang::SourceLocation clang::Cursor::get_source_location() const {
  return SourceLocation(clang_getCursorLocation(cx_cursor));
}

clang::SourceRange clang::Cursor::get_source_range() const {
  return SourceRange(clang_getCursorExtent(cx_cursor));
}

std::string clang::Cursor::get_spelling() const {
  return clang::to_string(clang_getCursorSpelling(cx_cursor));
}

std::string clang::Cursor::get_usr() const {
  return clang::to_string(clang_getCursorUSR(cx_cursor));
}

clang::Cursor clang::Cursor::get_referenced() const {
  return Cursor(clang_getCursorReferenced(cx_cursor));
}

clang::Cursor::operator bool() const {
  return !clang_Cursor_isNull(cx_cursor);
}

bool clang::Cursor::operator==(const Cursor& rhs) const {
  return get_usr()==rhs.get_usr();
}

//TODO: Is there a way to optimise this?
bool clang::Cursor::has_type() {
  auto referenced=clang_getCursorReferenced(cx_cursor);
  if(clang_Cursor_isNull(referenced))
    return false;
  auto type=clang_getCursorType(referenced);
  auto spelling=clang::to_string(clang_getTypeSpelling(type));
  return spelling!="";
}

std::string clang::Cursor::get_type() {
  std::string spelling;
  auto referenced=clang_getCursorReferenced(cx_cursor);
  if(!clang_Cursor_isNull(referenced)) {
    auto type=clang_getCursorType(referenced);
    spelling=clang::to_string(clang_getTypeSpelling(type));
    
#if CINDEX_VERSION_MAJOR==0 && CINDEX_VERSION_MINOR<32
    const std::string auto_str="auto";
    if(spelling.size()>=4 && std::equal(auto_str.begin(), auto_str.end(), spelling.begin())) {
      auto canonical_type=clang_getCanonicalType(clang_getCursorType(cx_cursor));
      auto canonical_spelling=clang::to_string(clang_getTypeSpelling(canonical_type));
      if(spelling.size()>5 && spelling[4]==' ' && spelling[5]=='&' && spelling!=canonical_spelling)
        return canonical_spelling+" &";
      else
        return canonical_spelling;
    }
    
    const std::string const_auto_str="const auto";
    if(spelling.size()>=10 && std::equal(const_auto_str.begin(), const_auto_str.end(), spelling.begin())) {
      auto canonical_type=clang_getCanonicalType(clang_getCursorType(cx_cursor));
      auto canonical_spelling=clang::to_string(clang_getTypeSpelling(canonical_type));
      if(spelling.size()>11 && spelling[10]==' ' && spelling[11]=='&' && spelling!=canonical_spelling)
        return canonical_spelling+" &";
      else
        return canonical_spelling;
    }
#endif
  }
  
  return spelling;
}

std::string clang::Cursor::get_brief_comments() {
  std::string comment_string;
  auto referenced=get_referenced();
  if(referenced) {
    comment_string=clang::to_string(clang_Cursor_getBriefCommentText(referenced.cx_cursor));
  }
  return comment_string;
}
