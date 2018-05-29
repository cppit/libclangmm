#include "tokens.h"
#include "utility.h"
#include <unordered_set>
#include <cstring>
#include <map>

clangmm::Tokens::Tokens(CXTranslationUnit &cx_tu, const SourceRange &range, bool annotate_tokens): cx_tu(cx_tu) {
  unsigned num_tokens;
  clang_tokenize(cx_tu, range.cx_range, &cx_tokens, &num_tokens);
  
  if(!annotate_tokens) {
    cx_cursors=std::unique_ptr<CXCursor[]>(new CXCursor[num_tokens]);
    for (unsigned i = 0; i < num_tokens; i++) {
      cx_cursors[i]=clang_getNullCursor();
      emplace_back(Token(cx_tu, cx_tokens[i], cx_cursors[i]));
    }
    return;
  }
  
  cx_cursors=std::unique_ptr<CXCursor[]>(new CXCursor[num_tokens]); // To avoid allocation with initialization
  clang_annotateTokens(cx_tu, cx_tokens, num_tokens, cx_cursors.get());
  
  bool tu_cursors=SourceRange(clang_getCursorExtent(clang_getTranslationUnitCursor(cx_tu))).get_start().get_path()==range.get_start().get_path();
  std::map<size_t, Offset> invalid_tokens;
  
  for (unsigned i = 0; i < num_tokens; i++) {
    if(cx_cursors[i].kind==CXCursor_DeclRefExpr) { // Temporary fix to a libclang bug
      auto real_cursor=clang_getCursor(cx_tu, clang_getTokenLocation(cx_tu, cx_tokens[i]));
      cx_cursors[i]=real_cursor;
    }
    // Corrects: when getting tokens from a header, FieldDecl tokens are getting ClassDecl or StructDecl cursors
    else if(!tu_cursors && (cx_cursors[i].kind==CXCursor_ClassDecl || cx_cursors[i].kind==CXCursor_StructDecl)) {
      Token token(cx_tu, cx_tokens[i], cx_cursors[i]);
      auto cursor=token.get_cursor();
      auto token_offsets=token.get_source_range().get_offsets();
      if(token_offsets.second!=cursor.get_source_range().get_offsets().second && token_offsets.first!=cursor.get_source_location().get_offset() && token.is_identifier())
        invalid_tokens.emplace(i, token_offsets.first);
    }
    
    emplace_back(Token(cx_tu, cx_tokens[i], cx_cursors[i]));
  }
  if(!tu_cursors && !invalid_tokens.empty()) {
    class VisitorData {
    public:
      Tokens *tokens;
      const std::string &path;
      std::map<size_t, Offset> &invalid_tokens;
      std::vector<Cursor> cursors;
    };
    VisitorData data{this, range.get_start().get_path(), invalid_tokens, {}};
    auto translation_unit_cursor = clang_getTranslationUnitCursor(cx_tu);
    clang_visitChildren(translation_unit_cursor, [](CXCursor cx_cursor, CXCursor cx_parent, CXClientData data_) {
      auto data=static_cast<VisitorData*>(data_);
      Cursor cursor(cx_cursor);
      if(cursor.get_source_location().get_path()==data->path)
        data->cursors.emplace_back(cursor);
      return CXChildVisit_Continue;
    }, &data);
    
    for(auto &cursor: data.cursors) {
      clang_visitChildren(cursor.cx_cursor, [](CXCursor cx_cursor, CXCursor cx_parent, CXClientData data_) {
        auto data=static_cast<VisitorData*>(data_);
        if(clang_getCursorKind(cx_cursor)==CXCursor_FieldDecl) {
          Cursor cursor(cx_cursor);
          auto clang_offset=cursor.get_source_location().get_offset();
          for(auto it=data->invalid_tokens.begin();it!=data->invalid_tokens.end();) {
            if(it->second==clang_offset) {
              (*data->tokens)[it->first].cx_cursor=cursor.cx_cursor;
              it=data->invalid_tokens.erase(it);
              if(data->invalid_tokens.empty())
                return CXChildVisit_Break;
              break;
            }
            else
              ++it;
          }
        }
        return CXChildVisit_Recurse;
      }, &data);
      if(invalid_tokens.empty())
        break;
    }
  }
}

clangmm::Tokens::~Tokens() {
  clang_disposeTokens(cx_tu, cx_tokens, size());
}

//This works across TranslationUnits. Similar tokens defined as tokens with equal canonical cursors. 
std::vector<std::pair<clangmm::Offset, clangmm::Offset> > clangmm::Tokens::get_similar_token_offsets(Cursor::Kind kind, const std::string &spelling,
                                                                                                     const std::unordered_set<std::string> &usrs) {
  std::vector<std::pair<Offset, Offset> > offsets;
  for(auto &token: *this) {
    if(token.is_identifier()) {
      auto referenced=token.get_cursor().get_referenced();
      if(referenced && Cursor::is_similar_kind(referenced.get_kind(), kind)) {
        bool equal_spelling=false;
        auto cx_string=clang_getTokenSpelling(cx_tu, token.cx_token);
        if(cx_string.data)
          equal_spelling=std::strcmp(static_cast<const char*>(cx_string.data), spelling.c_str())==0;
        clang_disposeString(cx_string);
        if(equal_spelling) {
          auto referenced_usrs=referenced.get_all_usr_extended();
          for(auto &usr: referenced_usrs) {
            if(usrs.count(usr)) {
              offsets.emplace_back(token.get_source_range().get_offsets());
              break;
            }
          }
        }
      }
    }
  }
  return offsets;
}
