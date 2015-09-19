#include "Cursor.h"
#include "Utility.h"

const clang::CursorKind clang::Cursor::get_kind() {
  return (CursorKind) clang_getCursorKind(this->cx_cursor);
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
    std::string auto_end="";
    //TODO fix const auto
    if((spelling.size()>=4 && spelling.substr(0, 4)=="auto")) {
      auto_end=spelling.substr(4);
      auto type=clang_getCanonicalType(clang_getCursorType(cx_cursor));
      spelling=clang::to_string(clang_getTypeSpelling(type));
      if(spelling.find(" ")==std::string::npos)
        spelling+=auto_end;
    }
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
