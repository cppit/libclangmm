#ifndef COMPLETIONSTRING_H_
#define COMPLETIONSTRING_H_
#include <clang-c/Index.h>
#include "CodeCompleteResults.h"

namespace clang {
  enum CompletionChunkKind {
    CompletionChunk_Optional,  CompletionChunk_TypedText,
    CompletionChunk_Text, CompletionChunk_Placeholder,
    CompletionChunk_Informative, CompletionChunk_CurrentParameter,
    CompletionChunk_LeftParen, CompletionChunk_RightParen,
    CompletionChunk_LeftBracket, CompletionChunk_RightBracket,
    CompletionChunk_LeftBrace, CompletionChunk_RightBrace,
    CompletionChunk_LeftAngle, CompletionChunk_RightAngle,
    CompletionChunk_Comma, CompletionChunk_ResultType,
    CompletionChunk_Colon, CompletionChunk_SemiColon,
    CompletionChunk_Equal, CompletionChunk_HorizontalSpace,
    CompletionChunk_VerticalSpace
  };

  class CompletionChunk {
  public:
    CompletionChunk(std::string chunk, CompletionChunkKind kind);
    const std::string& chunk() const { return chunk_; }
    const CompletionChunkKind& kind() const { return kind_; }
  private:
    std::string chunk_;
    CompletionChunkKind kind_;
  };

  class CompletionString {
  public:
    std::vector<CompletionChunk> get_chunks();
    std::string get_brief_comments();
    int get_num_chunks();
  private:
    explicit CompletionString(const CXCompletionString &str);
    CXCompletionString str_;
    friend CodeCompleteResults;
  };
}  // namespace clang
#endif  // COMPLETIONSTRING_H_
