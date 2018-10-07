# some argument checking:

# test_cmd is the command to run with all its arguments
#if(NOT test_cmd)
#    message(FATAL_ERROR "Variable test_cmd not defined")
#endif()
#
## output_blessed contains the name of the "blessed" output file
#if(NOT output_blessed)
#    message(FATAL_ERROR "Variable output_blessed not defined")
#endif()
#
## output_test contains the name of the output file the test_cmd will produce
#if(NOT output_test)
#    message(FATAL_ERROR "Variable output_test not defined")
#endif()

message(status "Hallo Welt")

#message(STATUS "running: \"${test_cmd}\"")
#
#execute_process(
#    COMMAND ${test_cmd}
#	RESULT_VARIABLE execution_not_successful
#	ERROR_VARIABLE stderr
#)

#execute_process(
#    COMMAND ${CMAKE_COMMAND} -E compare_files ${output_blessed} ${output_test}
#    RESULT_VARIABLE comparison_not_successful
#)

#if(execution_not_successful)
#    message(SEND_ERROR "execution of ${test_cmd} failed with: ${execution_not_successful}!\n" "${stderr}")
#endif()

#if(stderr)
#	message(SEND_ERROR "${stderr}")
#endif()

#if(comparison_not_successful)
#    message(FATAL_ERROR "${output_test} does not match $ {output_blessed}!")
#endif()
