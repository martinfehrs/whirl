# CMake generated Testfile for 
# Source directory: /home/martin/LL1/tests
# Build directory: /home/martin/LL1/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(static_look_ahead_buffer "/home/martin/LL1/build/tests/tests" "static_look_ahead_buffer")
add_test(is "/home/martin/LL1/build/tests/tests" "is")
add_test(is-not "/home/martin/LL1/build/tests/tests" "is-not")
add_test(is-one-of "/home/martin/LL1/build/tests/tests" "is-one-of")
add_test(sequential "/home/martin/LL1/build/examples/sequential")
set_tests_properties(sequential PROPERTIES  PASS_REGULAR_EXPRESSION "^-3[
	 ]*-3[
	 ]*-2[
	 ]*-1[
	 ]*0[
	 ]*-1[
	 ]*0[
	 ]*1[
	 ]*1[
	 ]*2[
	 ]*2[
	 ]*2[
	 ]*4[
	 ]*5[
	 ]*8[
	 ]*" RUN_SERIAL "1" WORKING_DIRECTORY "/home/martin/LL1/build/examples")
