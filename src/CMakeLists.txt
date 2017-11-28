add_library(clangmm
  code_complete_results.cc
  compilation_database.cc
  compile_command.cc
  compile_commands.cc
  completion_string.cc
  cursor.cc
  index.cc
  source_location.cc
  source_range.cc
  token.cc
  tokens.cc
  translation_unit.cc
  diagnostic.cc
  utility.cc
)

include_directories(${LIBCLANG_INCLUDE_DIRS})
target_link_libraries(clangmm ${LIBCLANG_LIBRARIES})
target_include_directories(clangmm PUBLIC ${LIBCLANG_INCLUDE_DIRS} .)

if(${NOT_SUB_PROJECT})
  install(TARGETS clangmm ARCHIVE DESTINATION lib)
  install(FILES
    clangmm.h
    code_complete_results.h
    compilation_database.h
    compile_command.h
    compile_commands.h
    completion_string.h
    cursor.h
    index.h
    source_location.h
    source_range.h
    token.h
    tokens.h
    translation_unit.h
    diagnostic.h
    utility.h
    DESTINATION include/libclangmm
  )
endif()
