#include "Index.h"

clangmm::Index::Index(int excludeDeclarationsFromPCH, int displayDiagnostics) {
  cx_index = clang_createIndex(excludeDeclarationsFromPCH, displayDiagnostics);
}

clangmm::Index::~Index() {
  clang_disposeIndex(cx_index);
}