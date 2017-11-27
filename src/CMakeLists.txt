add_library(clangmm
  CodeCompleteResults.cc
  CompilationDatabase.cc
  CompileCommand.cc     
  CompileCommands.cc    
  CompletionString.cc   
  Cursor.cc
  Index.cc              
  SourceLocation.cc     
  SourceRange.cc        
  Token.cc              
  Tokens.cc             
  TranslationUnit.cc
  Diagnostic.cc
  Utility.cc
)

include_directories(${LIBCLANG_INCLUDE_DIRS})
target_link_libraries(clangmm ${LIBCLANG_LIBRARIES})
target_include_directories(clangmm PUBLIC ${LIBCLANG_INCLUDE_DIRS} .)

if(${NOT_SUB_PROJECT})
  install(TARGETS clangmm ARCHIVE DESTINATION lib)
  install(FILES
    clangmm.h
    CodeCompleteResults.h
    CompilationDatabase.h
    CompileCommand.h
    CompileCommands.h
    CompletionString.h
    Cursor.h
    Index.h
    SourceLocation.h
    SourceRange.h
    Token.h
    Tokens.h
    TranslationUnit.h
    Diagnostic.h
    Utility.h
    DESTINATION include/libclangmm
  )
endif()
