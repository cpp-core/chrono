cmake_minimum_required (VERSION 3.22 FATAL_ERROR)
project (chrono VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET chrono)
  return()
endif()

# Configuration variables.
#
# Build the tests and set directories if this is the master project
#
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(CHRONO_TEST ON CACHE BOOL "Generate the tests.")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_DOCS_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
  
  # Have cmake output the compilation commands
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# Options for generating tests and documentation.
#
option(CHRONO_TEST "Generate the tests." OFF)
option(CHRONO_DOCS "Generate the docs." OFF)

# Add our dependencies
#
include(GNUInstallDirs)
find_package(string REQUIRED)
find_package(util REQUIRED)
find_package(stream REQUIRED)
find_package(date REQUIRED)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)

# Output configuration
#
message("-- chrono: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- chrono: Included from: ${CMAKE_SOURCE_DIR}")
message("-- chrono: CHRONO_TEST ${CHRONO_TEST}")
message("-- chrono: CHRONO_DOCS ${CHRONO_DOCS}")
message("-- chrono: CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")
message("-- chrono: string: ${string_DIR}")
message("-- chrono: util: ${util_DIR}")
message("-- chrono: stream: ${stream_DIR}")

# Compilation
#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)
add_compile_options(-stdlib=libc++)
add_compile_options(-O)
add_link_options(-stdlib=libc++)

set(SOURCES
  chrono/date
  chrono/date_stream
  chrono/duration
  chrono/lowres_clock
  chrono/time_of_day
  chrono/time_of_day_stream
  chrono/timepoint
  chrono/timepoint_stream
  )

foreach(NAME ${SOURCES})
  list(APPEND FILES "src/core/${NAME}.cpp")
endforeach()

add_library(chrono ${FILES})

set(LIBRARIES
  stream::stream
  date::date-tz
  fmt::fmt
  nlohmann_json::nlohmann_json)

target_link_libraries(chrono PUBLIC ${LIBRARIES})

target_include_directories(chrono
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Optionally configure the tests
#
if(CHRONO_TEST)
  enable_testing()
  if (NOT TARGET check)
    add_custom_target(check ${CMAKE_CTEST_COMMAND})
    add_custom_target(check_detail ${CMAKE_CTEST_COMMAND} -V)
  endif()
  add_subdirectory(test)
endif()

# Optionally configure the documentation
#
# if(FP_DOCS)
#   add_subdirectory(docs)
# endif()

# Installation
#
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/install_config.cmake)
install_config(chrono)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/core/chrono DESTINATION include/core)
