cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

cpp_core_find_package(googletest VERSION v1.12.0 EXCLUDE_FROM_ALL)
find_package(Threads REQUIRED)

set(TESTS
  chrono/date
  chrono/lowres_clock
  chrono/time_of_day
  chrono/timepoint
  )

set(TEST_LIBRARIES
  chrono
  GTest::gtest
  Threads::Threads)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configure_tests.cmake)
configure_tests("core" "${TEST_LIBRARIES}" ${TESTS})
