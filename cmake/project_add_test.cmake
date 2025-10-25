function(project_add_test target)
    enable_testing()
    project_add_executable(${target})
    add_test(${target} ${target})

    get_target_property(test_path ${target} RUNTIME_OUTPUT_DIRECTORY)
    list(APPEND PROJECT_TEST_EXECUTABLES ${test_path})
    set(PROJECT_TEST_EXECUTABLES "${PROJECT_TEST_EXECUTABLES}" PARENT_SCOPE)
endfunction()