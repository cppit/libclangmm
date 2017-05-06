#include "CompletionString.h"
#include "Utility.h"

clangmm::CompletionString::
CompletionString(const CXCompletionString &cx_completion_sting) : cx_completion_sting(cx_completion_sting) {}

bool clangmm::CompletionString::available() {
  return clang_getCompletionAvailability(cx_completion_sting) == CXAvailability_Available;
}

unsigned clangmm::CompletionString::get_num_chunks() {
    return clang_getNumCompletionChunks(cx_completion_sting);
}

std::vector<clangmm::CompletionChunk> clangmm::CompletionString::get_chunks() {
  std::vector<CompletionChunk> res;
  for (unsigned i = 0; i < get_num_chunks(); i++) {
    res.emplace_back(to_string(clang_getCompletionChunkText(cx_completion_sting, i)), static_cast<CompletionChunkKind> (clang_getCompletionChunkKind(cx_completion_sting, i)));
  }
  return res;
}

std::string clangmm::CompletionString::get_brief_comments() {
  return to_string(clang_getCompletionBriefComment(cx_completion_sting));
}

clangmm::CompletionChunk::CompletionChunk(std::string chunk, CompletionChunkKind kind) :
  chunk(chunk), kind(kind) { }
