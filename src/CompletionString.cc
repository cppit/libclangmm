#include "CompletionString.h"
#include "Utility.h"

clangmm::CompletionChunk::CompletionChunk(std::string text, CompletionChunkKind kind)
    : text(std::move(text)), kind(kind) {}

clangmm::CompletionString::CompletionString(const CXCompletionString &cx_completion_sting)
    : cx_completion_sting(cx_completion_sting) {}

bool clangmm::CompletionString::available() const {
  return clang_getCompletionAvailability(cx_completion_sting) == CXAvailability_Available;
}

unsigned clangmm::CompletionString::get_num_chunks() const {
  return clang_getNumCompletionChunks(cx_completion_sting);
}

std::vector<clangmm::CompletionChunk> clangmm::CompletionString::get_chunks() const {
  std::vector<CompletionChunk> chunks;
  for(unsigned i = 0; i < get_num_chunks(); ++i)
    chunks.emplace_back(to_string(clang_getCompletionChunkText(cx_completion_sting, i)),
                        static_cast<CompletionChunkKind>(clang_getCompletionChunkKind(cx_completion_sting, i)));
  return chunks;
}

std::string clangmm::CompletionString::get_brief_comment() const {
  return to_string(clang_getCompletionBriefComment(cx_completion_sting));
}
