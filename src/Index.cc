#include "Index.h"

clang::Index::
Index(int excludeDeclarationsFromPCH, int displayDiagnostics) {
  index_ = clang_createIndex(excludeDeclarationsFromPCH,
                             displayDiagnostics);
}
