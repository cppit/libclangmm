#include "Tokens.h"
#include "Utility.h"
#include <unordered_set>
#include <cstring>

clangmm::Tokens::Tokens(CXTranslationUnit &cx_tu, const SourceRange &range): cx_tu(cx_tu) {
  unsigned num_tokens;
  clang_tokenize(cx_tu, range.cx_range, &cx_tokens, &num_tokens);
  cx_cursors=std::unique_ptr<CXCursor[]>(new CXCursor[num_tokens]); // To avoid allocation with initialization
  clang_annotateTokens(cx_tu, cx_tokens, num_tokens, cx_cursors.get());
  for (unsigned i = 0; i < num_tokens; i++) {
    if(cx_cursors[i].kind==CXCursor_DeclRefExpr) { // Temporary fix to a libclang bug
      auto real_cursor=clang_getCursor(cx_tu, clang_getTokenLocation(cx_tu, cx_tokens[i]));
      cx_cursors[i]=real_cursor;
    }
    // Corrects: when getting tokens from a header, FieldDecl tokens are getting ClassDecl or StructDecl cursors
    else if(cx_cursors[i].kind==CXCursor_ClassDecl || cx_cursors[i].kind==CXCursor_StructDecl) {
      Token token(cx_tu, cx_tokens[i], cx_cursors[i]);
      auto cursor=token.get_cursor();
      auto token_offsets=token.get_source_range().get_offsets();
      if(token_offsets.second!=cursor.get_source_range().get_offsets().second && token_offsets.first!=cursor.get_source_location().get_offset() && token.is_identifier()) {
        class VisitorData {
        public:
          std::string path;
          bool found_path;
          std::pair<Offset, Offset> offsets;
          CXCursor found_cursor;
        };
        VisitorData data{token.get_source_location().get_path(), false, token_offsets, clang_getNullCursor()};
        auto translation_unit_cursor = clang_getTranslationUnitCursor(cx_tu);
        clang_visitChildren(translation_unit_cursor, [](CXCursor cx_cursor, CXCursor cx_parent, CXClientData data_) {
          auto data=static_cast<VisitorData*>(data_);
          Cursor cursor(cx_cursor);
          if(data->found_path || cursor.get_source_location().get_path()==data->path) {
            data->found_path=true;
            if(cursor.get_source_range().get_offsets().second==data->offsets.second && cursor.get_source_location().get_offset()==data->offsets.first) {
              data->found_cursor=cx_cursor;
              return CXChildVisit_Break;
            }
            return CXChildVisit_Recurse;
          }
          return CXChildVisit_Continue;
        }, &data);
        
        if(!clang_Cursor_isNull(data.found_cursor))
          cx_cursors[i]=data.found_cursor;
      }
    }
    emplace_back(Token(cx_tu, cx_tokens[i], cx_cursors[i]));
  }
}

clangmm::Tokens::~Tokens() {
  clang_disposeTokens(cx_tu, cx_tokens, size());
}

//This works across TranslationUnits. Similar tokens defined as tokens with equal canonical cursors. 
std::vector<std::pair<clangmm::Offset, clangmm::Offset> > clangmm::Tokens::get_similar_token_offsets(const std::string &spelling,
                                                                                                     const std::unordered_set<std::string> &usrs) {
  std::vector<std::pair<Offset, Offset> > offsets;
  for(auto &token: *this) {
    if(token.is_identifier()) {
      auto referenced=token.get_cursor().get_referenced();
      if(referenced) {
        const char *strstr_ptr=nullptr;
        auto cx_string=clang_getTokenSpelling(cx_tu, token.cx_token);
        if(cx_string.data)
          strstr_ptr=std::strstr(static_cast<const char*>(cx_string.data), spelling.c_str());
        clang_disposeString(cx_string);
        if(strstr_ptr) {
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
