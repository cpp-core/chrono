cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

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

configure_tests("core" "${TEST_LIBRARIES}" ${TESTS})
