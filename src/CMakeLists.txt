set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(header_files
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
  )
set(cc_files
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

add_library(${project_name} SHARED ${header_files} ${cc_files})

include_directories(${LIBCLANG_INCLUDE_DIRS})
target_link_libraries(${project_name} ${LIBCLANG_LIBRARIES})

install(TARGETS ${project_name} RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
install(FILES ${header_files} DESTINATION include/libclangmm)
