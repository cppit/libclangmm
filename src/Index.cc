#include "Index.h"

clang::Index::
Index(int excludeDeclarationsFromPCH, int displayDiagnostics) {
  cx_index = clang_createIndex(excludeDeclarationsFromPCH,
                             displayDiagnostics);
}
