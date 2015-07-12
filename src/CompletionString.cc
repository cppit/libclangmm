#include "CompletionString.h"

clang::CompletionString::
CompletionString(const CXCompletionString &str) {
  str_ = str;
}

bool clang::CompletionString::available() {
  return clang_getCompletionAvailability(str_) == CXAvailability_Available;
}

int clang::CompletionString::get_num_chunks() {
    return clang_getNumCompletionChunks(str_);
}

std::vector<clang::CompletionChunk> clang::CompletionString::get_chunks() {
  std::vector<clang::CompletionChunk> res;
  for (size_t i = 0; i < get_num_chunks(); i++) {
    auto cxstr=clang_getCompletionChunkText(str_, i);
    res.emplace_back(clang_getCString(cxstr), static_cast<CompletionChunkKind> (clang_getCompletionChunkKind(str_, i)));
    clang_disposeString(cxstr);
  }
  return res;
}

std::string clang::CompletionString::get_brief_comments() {
  std::string brief_comments;
  auto cxstr=clang_getCompletionBriefComment(str_);
  if(cxstr.data!=NULL) {
    brief_comments=clang_getCString(cxstr);
    clang_disposeString(cxstr);
  }
  return brief_comments;
}

clang::CompletionChunk::
CompletionChunk(std::string chunk, clang::CompletionChunkKind kind) :
  chunk(chunk), kind(kind) { }
