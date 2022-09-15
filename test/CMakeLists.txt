cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

find_package(googletest REQUIRED)
find_package(Threads REQUIRED)

set(TESTS
  chrono/date
  chrono/time_of_day
  chrono/timepoint
  )

set(TEST_LIBRARIES
  cxx_core_chrono
  cxx_core_string::cxx_core_string
  cxx_core_util::cxx_core_util
  cxx_coro_stream::cxx_coro_stream
  googletest::cxx-googletest
  Threads::Threads)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configure_tests.cmake)
configure_tests("core" "${TEST_LIBRARIES}" ${TESTS})
