#include "completion_string.h"
#include "utility.h"

clangmm::CompletionChunk::CompletionChunk(std::string text, CompletionChunkKind kind)
    : text(std::move(text)), kind(kind) {}

std::vector<clangmm::CompletionChunk> clangmm::CompletionString::get_chunks() const {
  std::vector<CompletionChunk> chunks;
  for(unsigned i = 0; i < get_num_chunks(); ++i)
    chunks.emplace_back(to_string(clang_getCompletionChunkText(cx_completion_string, i)),
                        static_cast<CompletionChunkKind>(clang_getCompletionChunkKind(cx_completion_string, i)));
  return chunks;
}

clangmm::CompletionString::CompletionString(const CXCompletionString &cx_completion_string)
    : cx_completion_string(cx_completion_string) {}

bool clangmm::CompletionString::available() const {
  return clang_getCompletionAvailability(cx_completion_string) == CXAvailability_Available;
}

unsigned clangmm::CompletionString::get_num_chunks() const {
  return clang_getNumCompletionChunks(cx_completion_string);
}

std::string clangmm::CompletionString::get_brief_comment() const {
  return to_string(clang_getCompletionBriefComment(cx_completion_string));
}

// Several workarounds has been added due to clang_getCursorCompletionString result being different from normal completion strings
clangmm::Cursor clangmm::CompletionString::get_cursor(CXTranslationUnit &tu) const {
  class VisitorData {
  public:
    std::vector<std::string> &completion_chunks;
    std::vector<std::string> parent_parts;
    clangmm::Cursor found_cursor;
  };
  class ChunkString {
  public:
    static void remove_template_argument_and_namespace(std::string &chunk) {
      size_t pos1, pos2;
      if((pos1=chunk.find('<'))!=std::string::npos && (pos2=chunk.rfind('>'))!=std::string::npos)
        chunk=chunk.substr(0, pos1)+chunk.substr(pos2+1);
      if((pos2=chunk.rfind("::"))!=std::string::npos) {
        pos1=pos2-1;
        while(pos1!=std::string::npos && ((chunk[pos1]>='a' && chunk[pos1]<='z') || (chunk[pos1]>='A' && chunk[pos1]<='Z') ||
                                          (chunk[pos1]>='0' && chunk[pos1]<='9') || chunk[pos1]==':' || chunk[pos1]=='_'))
          --pos1;
        chunk=chunk.substr(0, pos1+1)+chunk.substr(pos2+2);
      }
    }
  };
  std::vector<std::string> chunks;
  for(unsigned i=0;i<clang_getNumCompletionChunks(cx_completion_string);++i) {
    auto kind = clang_getCompletionChunkKind(cx_completion_string, i);
    if(kind != CXCompletionChunk_Optional && kind != CXCompletionChunk_Informative) {
      auto chunk=clangmm::to_string(clang_getCompletionChunkText(cx_completion_string, i));
      ChunkString::remove_template_argument_and_namespace(chunk);
      chunks.emplace_back(chunk);
    }
  }
  auto parent=clangmm::to_string(clang_getCompletionParent(cx_completion_string, nullptr));
  std::vector<std::string> parent_parts;
  if(!parent.empty()) {
    size_t pos=0;
    size_t last_pos=0;
    while((pos=parent.find("::", pos))!=std::string::npos) {
      parent_parts.emplace_back(parent.substr(last_pos, pos-last_pos));
      pos+=2;
      last_pos=pos;
    }
    parent_parts.emplace_back(parent.substr(last_pos));
  }
  
  VisitorData visitor_data{chunks, parent_parts, clangmm::Cursor()};
  
  clang_visitChildren(clang_getTranslationUnitCursor(tu), [](CXCursor cx_cursor, CXCursor cx_parent, CXClientData cx_data) {
    auto data = static_cast<VisitorData *>(cx_data);
    
    bool equal=true;
    auto cx_tmp_cursor=cx_parent;
    if(clang_getCursorKind(cx_tmp_cursor)!=CXCursorKind::CXCursor_TranslationUnit) {
      int c=0;
      auto it=data->parent_parts.rbegin();
      for(;it!=data->parent_parts.rend();++it) {
        auto name=clangmm::to_string(clang_getCursorDisplayName(cx_tmp_cursor));
        size_t pos;
        if((pos=name.find('<'))!=std::string::npos)
          name=name.substr(0, pos);
        if(name!=*it) {
          equal=false;
          break;
        }
        cx_tmp_cursor=clang_getCursorSemanticParent(cx_tmp_cursor);
        if(clang_getCursorKind(cx_tmp_cursor)==CXCursorKind::CXCursor_TranslationUnit) {
          ++it;
          break;
        }
        ++c;
      }
      if(it!=data->parent_parts.rend())
        equal=false;
    }
    else if(!data->parent_parts.empty())
      return CXChildVisit_Recurse;
    
    if(equal) {
      auto completion_string = clang_getCursorCompletionString(cx_cursor);
      auto num_completion_chunks=clang_getNumCompletionChunks(completion_string);
      if(num_completion_chunks>=data->completion_chunks.size()) {
        bool equal=true;
        for(unsigned i=0;i<data->completion_chunks.size() && i<num_completion_chunks;++i) {
          auto kind = clang_getCompletionChunkKind(completion_string, i);
          if(kind != CXCompletionChunk_Optional && kind != CXCompletionChunk_Informative) {
            auto chunk=clangmm::to_string(clang_getCompletionChunkText(completion_string, i));
            ChunkString::remove_template_argument_and_namespace(chunk);
            if(data->completion_chunks[i]!=chunk) {
              equal=false;
              break;
            }
          }
        }
        if(equal) {
          data->found_cursor=cx_cursor;
          return CXChildVisit_Break;
        }
      }
    }
  
    return CXChildVisit_Recurse;
  }, &visitor_data);
  
  return Cursor(visitor_data.found_cursor);
}
