cmake_minimum_required (VERSION 3.24 FATAL_ERROR)
project (chrono VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET chrono OR TARGET chrono::chrono)
  return()
endif()

# If this is the current project.
#
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  # Laod the cmake recipes and utilities.
  #
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/load_cmake_helpers.cmake)

  # Options for generating tests and documentation.
  #
  option(CHRONO_TEST "Generate the tests." ON)
  option(CHRONO_DOCS "Generate the docs." OFF)


  # compile_commands.json
  #
  symlink_compile_commands()

  # Convenience targets for git operations
  #
  make_submodule_pull_target()
  
else()
  option(CHRONO_TEST "Generate the tests." OFF)
  option(CHRONO_DOCS "Generate the docs." OFF)
endif()

# Put executables in the top-level binary directory
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Output configuration
#
message("-- chrono: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- chrono: Included from: ${CMAKE_SOURCE_DIR}")
message("-- chrono: Install prefix: ${CMAKE_INSTALL_PREFIX}")
message("-- chrono: test ${CHRONO_TEST}")
message("-- chrono: docs ${CHRONO_DOCS}")

# Setup compilation before adding dependencies
#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)
add_compile_options(-stdlib=libc++)
add_compile_options(-O)
add_link_options(-stdlib=libc++)

# Add our dependencies
#
add_util()
add_date()

# Build the library
#
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
add_library(chrono::chrono ALIAS chrono)

# Define the header files for this library.
#
file(GLOB_RECURSE PUBLIC_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
target_sources(chrono PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${PUBLIC_INCLUDE_FILES})

target_link_libraries(chrono PUBLIC util::util date::date-tz)

# Optionally configure the tests
#
if(CHRONO_TEST)
  add_gtest()
  add_subdirectory(test)
endif()

# Optionally configure the documentation
#
# if(FP_DOCS)
#   add_subdirectory(docs)
# endif()

# Installation
#
install_config(chrono)
