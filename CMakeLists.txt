cmake_minimum_required (VERSION 3.22 FATAL_ERROR)
project (chrono VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET chrono OR TARGET chrono::chrono)
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
  
  # Have cmake output the compilation commands and create a link to it
  # from the project root directory so the language server can find
  # it.
  #
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()

# Options for generating tests and documentation.
#
option(CHRONO_TEST "Generate the tests." OFF)
option(CHRONO_DOCS "Generate the docs." OFF)

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
include(cmake/cpp_core_find_package.cmake)
cpp_core_find_package(util)

set(USE_SYSTEM_TZ_DB ON CACHE INTERNAL "")
set(BUILD_TZ_LIB ON CACHE INTERNAL "")
cpp_core_find_package(date VERSION v3.0.1)

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
target_link_libraries(chrono PUBLIC util::util date::date-tz)

# Define the header files for this library.
#
file(GLOB_RECURSE PUBLIC_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
target_sources(chrono PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${PUBLIC_INCLUDE_FILES})

# Optionally configure the tests
#
if(CHRONO_TEST)
  enable_testing()
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
