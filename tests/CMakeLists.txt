set(project_tests ${project_name}_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLIBCLANGMM_TESTS_PATH=\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\"")

include_directories(${LIBCLANG_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/src")

add_executable(code_complete_results_test code_complete_results_test.cc)
target_link_libraries(code_complete_results_test ${LIBCLANG_LIBRARIES} clangmm)
add_test(code_complete_results_test code_complete_results_test)

add_executable(completion_string_test completion_string_test.cc)
target_link_libraries(completion_string_test ${LIBCLANG_LIBRARIES} clangmm)
add_test(completion_string_test completion_string_test)

add_executable(cursor_test cursor_test.cc)
target_link_libraries(cursor_test ${LIBCLANG_LIBRARIES} clangmm)
add_test(cursor_test cursor_test)

add_executable(diagnostics_test diagnostics_test.cc)
target_link_libraries(diagnostics_test ${LIBCLANG_LIBRARIES} clangmm)
add_test(diagnostics_test diagnostics_test)

add_executable(source_location_test source_location_test.cc)
target_link_libraries(source_location_test ${LIBCLANG_LIBRARIES} clangmm)
add_test(source_location_test source_location_test)

add_executable(token_test token_test.cc)
target_link_libraries(token_test ${LIBCLANG_LIBRARIES} clangmm)
add_test(token_test token_test)

add_executable(translation_unit_test translation_unit_test.cc)
target_link_libraries(translation_unit_test ${LIBCLANG_LIBRARIES} clangmm)
add_test(translation_unit_test translation_unit_test)
