#include "cursor.h"
#include "utility.h"
#include <algorithm>

std::string clangmm::Cursor::Type::get_spelling() const {
  return to_string(clang_getTypeSpelling(cx_type));
}

clangmm::Cursor::Type clangmm::Cursor::Type::get_result() const {
  return Type(clang_getResultType(cx_type));
}

clangmm::Cursor clangmm::Cursor::Type::get_cursor() const {
  return Cursor(clang_getTypeDeclaration(cx_type));
}

bool clangmm::Cursor::Type::operator==(const Cursor::Type& rhs) const {
  return clang_equalTypes(cx_type, rhs.cx_type);
}

clangmm::Cursor::Kind clangmm::Cursor::get_kind() const {
  return static_cast<Kind>(clang_getCursorKind(cx_cursor));
}

bool clangmm::Cursor::is_similar_kind(Kind kind, Kind other_kind) {
  auto is_function_or_method=[](Kind kind) {
    if(kind==Kind::FunctionDecl || kind==Kind::CXXMethod || kind==Kind::FunctionTemplate)
      return true;
    return false;
  };
  auto is_class_or_struct=[](Kind kind) {
    if(kind==Kind::ClassDecl || kind==Kind::StructDecl || kind==Kind::ClassTemplate ||
       kind==Cursor::Kind::Constructor || kind==Cursor::Kind::Destructor || kind==Cursor::Kind::FunctionTemplate)
      return true;
    return false;
  };
  if(kind==Kind::FunctionTemplate)
    return is_function_or_method(other_kind) || is_class_or_struct(other_kind);
  if(is_function_or_method(kind))
    return is_function_or_method(other_kind);
  if(is_class_or_struct(kind))
    return is_class_or_struct(other_kind);
  return kind==other_kind;
}

clangmm::Cursor::Type clangmm::Cursor::get_type() const {
  return Type(clang_getCursorType(cx_cursor));
}

clangmm::SourceLocation clangmm::Cursor::get_source_location() const {
  return SourceLocation(clang_getCursorLocation(cx_cursor));
}

clangmm::SourceRange clangmm::Cursor::get_source_range() const {
  return SourceRange(clang_getCursorExtent(cx_cursor));
}

std::string clangmm::Cursor::get_spelling() const {
  return to_string(clang_getCursorSpelling(cx_cursor));
}

std::string clangmm::Cursor::get_display_name() const {
  return to_string(clang_getCursorDisplayName(cx_cursor));
}

std::string clangmm::Cursor::get_token_spelling() const {
  auto spelling=get_spelling();
  for(size_t i=0;i<spelling.size();++i) {
    if(spelling[i]=='<' || spelling[i]=='(') {
      if(i>0 && spelling[0]=='~')
        return spelling.substr(1, i-1);
      return spelling.substr(0, i);
    }
  }
  if(!spelling.empty() && spelling[0]=='~')
    return spelling.substr(1);
  return spelling;
}

std::string clangmm::Cursor::get_usr() const {
  return to_string(clang_getCursorUSR(cx_cursor));
}

std::string clangmm::Cursor::get_usr_extended() const {
  if(!is_valid_kind())
    return std::string();
  
  auto cursor=*this;
  auto kind=cursor.get_kind();
  // If constructor, destructor or function template, and the token spelling is equal, set cursor to parent
  if(kind==Cursor::Kind::Constructor || kind==Cursor::Kind::Destructor ||
     kind==Cursor::Kind::FunctionTemplate) {
    auto parent=cursor.get_semantic_parent();
    auto parent_kind=parent.get_kind();
    if((parent_kind==Cursor::Kind::ClassDecl || parent_kind==Cursor::Kind::StructDecl || parent_kind==Cursor::Kind::ClassTemplate) &&
       cursor.get_token_spelling()==parent.get_token_spelling())
      cursor=parent;
  }
  
  std::string usr=cursor.get_token_spelling();
  auto parent=cursor.get_semantic_parent();
  while((kind=parent.get_kind())!=Kind::TranslationUnit && parent.is_valid_kind()) {
    if(kind==Kind::CXXMethod || kind==Kind::FunctionDecl || kind==Kind::FunctionTemplate ||
       kind==Kind::Constructor || kind==Kind::Destructor) {
      auto canonical=cursor.get_canonical();
      auto location=canonical.get_source_location();
      auto offset=location.get_offset();
      return std::to_string(offset.line)+':'+std::to_string(offset.index)+':'+location.get_path();
    }
    usr+=':'+parent.get_token_spelling();
    parent=parent.get_semantic_parent();
  }
  return usr;
}

std::unordered_set<std::string> clangmm::Cursor::get_all_usr_extended() const {
  std::unordered_set<std::string> usrs;
  if(get_kind()==Kind::CXXMethod) {
    class Recursive {
    public:
      static void overridden(std::unordered_set<std::string> &usrs, const Cursor &cursor) {
        usrs.emplace(cursor.get_usr_extended());
        CXCursor *cursors;
        unsigned size;
        clang_getOverriddenCursors(cursor.cx_cursor, &cursors, &size);
        for(unsigned c=0;c<size;++c)
          overridden(usrs, cursors[c]);
        clang_disposeOverriddenCursors(cursors);
      }
    };
    Recursive::overridden(usrs, *this);
    return usrs;
  }
  else {
    usrs.emplace(get_usr_extended());
    return usrs;
  }
}

clangmm::Cursor clangmm::Cursor::get_referenced() const {
  return Cursor(clang_getCursorReferenced(cx_cursor));
}

clangmm::Cursor clangmm::Cursor::get_canonical() const {
  return Cursor(clang_getCanonicalCursor(cx_cursor));
}

clangmm::Cursor clangmm::Cursor::get_definition() const {
  return Cursor(clang_getCursorDefinition(cx_cursor));
}

clangmm::Cursor clangmm::Cursor::get_semantic_parent() const {
  return Cursor(clang_getCursorSemanticParent(cx_cursor));
}

std::vector<clangmm::Cursor> clangmm::Cursor::get_arguments() const {
  std::vector<Cursor> cursors;
  auto size=clang_Cursor_getNumArguments(cx_cursor);
  for(int c=0;c<size;++c)
    cursors.emplace_back(clang_Cursor_getArgument(cx_cursor, c));
  return cursors;
}

std::vector<clangmm::Cursor> clangmm::Cursor::get_all_overridden_cursors() const {
  std::vector<Cursor> result;
  if(get_kind()!=Kind::CXXMethod)
    return result;
  
  class Recursive {
  public:
    static void overridden(std::vector<Cursor> &result, const Cursor &cursor, int depth) {
      if(depth>0)
        result.emplace_back(cursor);
      CXCursor *cursors;
      unsigned size;
      clang_getOverriddenCursors(cursor.cx_cursor, &cursors, &size);
      for(unsigned c=0;c<size;++c)
        overridden(result, cursors[c], depth+1);
      clang_disposeOverriddenCursors(cursors);
    }
  };
  Recursive::overridden(result, *this, 0);
  return result;
}

clangmm::Cursor::operator bool() const {
  return !clang_Cursor_isNull(cx_cursor);
}

bool clangmm::Cursor::operator==(const Cursor& rhs) const {
  return clang_equalCursors(cx_cursor, rhs.cx_cursor);
}

bool clangmm::Cursor::is_valid_kind() const {
  auto referenced=clang_getCursorReferenced(cx_cursor);
  if(clang_Cursor_isNull(referenced))
    return false;
  auto kind=get_kind();
  return kind>Kind::UnexposedDecl && (kind<Kind::FirstInvalid || kind>Kind::LastInvalid);
}

std::string clangmm::Cursor::get_type_description() const {
  std::string spelling;
  auto referenced=clang_getCursorReferenced(cx_cursor);
  if(!clang_Cursor_isNull(referenced)) {
    auto type=clang_getCursorType(referenced);
    spelling=to_string(clang_getTypeSpelling(type));
    
#if CINDEX_VERSION_MAJOR==0 && CINDEX_VERSION_MINOR<32
    const std::string auto_str="auto";
    if(spelling.size()>=4 && std::equal(auto_str.begin(), auto_str.end(), spelling.begin())) {
      auto canonical_type=clang_getCanonicalType(clang_getCursorType(cx_cursor));
      auto canonical_spelling=to_string(clang_getTypeSpelling(canonical_type));
      if(spelling.size()>5 && spelling[4]==' ' && spelling[5]=='&' && spelling!=canonical_spelling)
        return canonical_spelling+" &";
      else
        return canonical_spelling;
    }
    
    const std::string const_auto_str="const auto";
    if(spelling.size()>=10 && std::equal(const_auto_str.begin(), const_auto_str.end(), spelling.begin())) {
      auto canonical_type=clang_getCanonicalType(clang_getCursorType(cx_cursor));
      auto canonical_spelling=to_string(clang_getTypeSpelling(canonical_type));
      if(spelling.size()>11 && spelling[10]==' ' && spelling[11]=='&' && spelling!=canonical_spelling)
        return canonical_spelling+" &";
      else
        return canonical_spelling;
    }
#endif
  }
  
  if(spelling.empty())
    return get_spelling();
  
  return spelling;
}

std::string clangmm::Cursor::get_brief_comments() const {
  std::string comment_string;
  auto referenced=get_referenced();
  if(referenced) {
    comment_string=to_string(clang_Cursor_getBriefCommentText(referenced.cx_cursor));
  }
  return comment_string;
}
