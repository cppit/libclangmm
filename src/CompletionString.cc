#include "CompletionString.h"

clang::CompletionString::
CompletionString(const CXCompletionString &str) {
  str_ = str;
}

int clang::CompletionString::
get_num_chunks() {
  if (clang_getCompletionAvailability(str_) == CXAvailability_Available)
    return clang_getNumCompletionChunks(str_);
  else
    return 0;
}

std::vector<clang::CompletionChunk> clang::CompletionString::
get_chunks() {
  std::vector<clang::CompletionChunk> res;
  if (clang_getCompletionAvailability(str_) == CXAvailability_Available) {
    for (auto i = 0; i < get_num_chunks(); i++) {
      res.emplace_back(clang_getCString(clang_getCompletionChunkText(str_, i)),
		       static_cast<CompletionChunkKind>
		       (clang_getCompletionChunkKind(str_, i)));
    }
  }
  return res;
}

clang::CompletionChunk::
CompletionChunk(std::string chunk, clang::CompletionChunkKind kind) :
  chunk_(chunk), kind_(kind) { }
