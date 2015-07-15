#include "CompletionString.h"

clang::CompletionString::
CompletionString(const CXCompletionString &cx_str) : cx_str(cx_str) {}

bool clang::CompletionString::available() {
  return clang_getCompletionAvailability(cx_str) == CXAvailability_Available;
}

int clang::CompletionString::get_num_chunks() {
    return clang_getNumCompletionChunks(cx_str);
}

std::vector<clang::CompletionChunk> clang::CompletionString::get_chunks() {
  std::vector<clang::CompletionChunk> res;
  for (size_t i = 0; i < get_num_chunks(); i++) {
    auto cxstr=clang_getCompletionChunkText(cx_str, i);
    res.emplace_back(clang_getCString(cxstr), static_cast<CompletionChunkKind> (clang_getCompletionChunkKind(cx_str, i)));
    clang_disposeString(cxstr);
  }
  return res;
}

std::string clang::CompletionString::get_brief_comments() {
  std::string brief_comments;
  auto cxstr=clang_getCompletionBriefComment(cx_str);
  if(cxstr.data!=NULL) {
    brief_comments=clang_getCString(cxstr);
    clang_disposeString(cxstr);
  }
  return brief_comments;
}

clang::CompletionChunk::
CompletionChunk(std::string chunk, clang::CompletionChunkKind kind) :
  chunk(chunk), kind(kind) { }
