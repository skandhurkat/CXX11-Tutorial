# CMakeLists.txt
# Copyright 2013-14 Skand Hurkat

# This file is part of CXX11-Tutorial.
#
# The program may not be used, modified, shared, or redistributed in any
# form without express permission from the copyright holder.
#
# The program is provided AS IS and WITHOUT ANY WARRANTY, without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

cmake_minimum_required(VERSION 2.8.10)
project("C++ 11 Tutorial")
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

include(CheckCXXCompilerFlag)

function(status status_msg_list)
  set(status_msg "")
  foreach(arg ${ARGV})
    set(status_msg "${status_msg} ${arg}")
  endforeach()
  message(STATUS ${status_msg})
endfunction()

option(NATIVE_ARCH "Build for native architecture" OFF)

find_package(Threads REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
if(WIN32)
  set(CMAKE_INSTALL_RPATH ${PROJECT_BINARY_DIR}/bin)
else()
  set(CMAKE_INSTALL_RPATH ${PROJECT_BINARY_DIR}/lib)
endif()

string(TOUPPER ${CMAKE_BUILD_TYPE} TOUPPER_BUILD_TYPE)
if(TOUPPER_BUILD_TYPE STREQUAL DEBUG)
  if(MSVC)
    add_definitions(/DDEBUG)
  else()
    add_definitions(-DDEBUG)
  endif()
elseif(TOUPPER_BUILD_TYPE STREQUAL RELWITHDEBINFO
      OR TOUPPER_BUILD_TYPE STREQUAL RELEASE)
  if(MSVC)
    add_definitions(/DNDEBUG)
  else()
    add_definitions(-DNDEBUG)
  endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  if(CMAKE_CXX_COMPILER_VERSION GREATER 4.7 OR
     CMAKE_CXX_COMPILER_VERSION EQUAL 4.7)
    option(ENABLE_PROFILING "Enables the profiling flag in debug mode" OFF)
    set(CMAKE_CXX_FLAGS "-std=c++11 -pedantic -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
    if(TOUPPER_BUILD_TYPE STREQUAL RELEASE)
      set(ENABLE_PROFILING OFF)
    endif()
    if(ENABLE_PROFILING)
      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg")
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
          "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -pg")
    endif()
    if(NATIVE_ARCH)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    endif()
  else()
	  message(FATAL_ERROR
	  "This project requires g++ 4.7 or higher")
  endif()
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
  set(ENABLE_PROFILING OFF)
  check_cxx_compiler_flag("-std=c++11" CXX11_SUPPORTED)
  check_cxx_compiler_flag("-stdlib=libc++" CXX11_LIBS_SUPPORTED)
  if(CXX11_SUPPORTED AND CXX11_LIBS_SUPPORTED)
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Weverything -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -gline-tables-only")
  else()
    message(FATAL_ERROR "Compiler does not appear to support C++11."
                        " This project requires C++11")
  endif()
elseif(MSVC AND MSVC_VERSION GREATER 1600)
# Nothing special needed for MSVC
# Just enable all warnings for debug and set NDEBUG for release
  set(ENABLE_PROFILING OFF)
  set(CMAKE_CXX_FLAGS "/Wall /WL /Wp64")
  set(CMAKE_CXX_FLAGS_DEBUG "/Od")
  set(CMAKE_CXX_FLAGS_RELEASE "/O2")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2")
else()
  message(FATAL_ERROR
  "Your compiler probably does not support C++11. This project requires C++11")
endif()

if(NOT CMAKE_COMPILER_IS_GNUCXX)
  set(NATIVE_ARCH OFF)
  set(ENABLE_VECTORISATION OFF)
endif()

########################################################################
# Add source files here                                                #
########################################################################

add_executable(microcounter microcounter.cpp)

########################################################################

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "A tutorial for C++ 11")
set(CPACK_PACKAGE_VENDOR "Skand Hurkat")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/README.md)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT ${CPACK_PACKAGE_VENDOR})
if(INSTALL_EXAMPLES)
  set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
endif()
include(CPack)

status("")
status("General configuration for ${CMAKE_PROJECT_NAME}")
status("  Platform:")
status("    Host                : ${CMAKE_HOST_SYSTEM_NAME}"
                                "${CMAKE_HOST_SYSTEM_VERSION}"
                                "${CMAKE_HOST_SYSTEM_PROCESSOR}")
status("    Target              : ${CMAKE_SYSTEM_NAME}"
                                "${CMAKE_SYSTEM_VERSION}"
                                "${CMAKE_SYSTEM_PROCESSOR}")
status("    CMake               : ${CMAKE_VERSION}")
status("    CMake generator     : ${CMAKE_GENERATOR}")
status("    CMake build tool    : ${CMAKE_BUILD_TOOL}")
status("  Build settings:")
status("    Build type          : ${CMAKE_BUILD_TYPE}")
status("    C++ Compiler        : ${CMAKE_CXX_COMPILER}")
status("    Profiling           : ${ENABLE_PROFILING}")
status("    Native Architecture : ${NATIVE_ARCH}")
status("    C++ Flags           : ${CMAKE_CXX_FLAGS}"
                                "${CMAKE_CXX_FLAGS_${TOUPPER_BUILD_TYPE}}")
unset(TOUPPER_BUILD_TYPE)
get_directory_property(CompileDefs COMPILE_DEFINITIONS)
status("    Compile definitions : ${CompileDefs}")
unset(CompileDefs)
status("    Install Location    : ${CMAKE_INSTALL_PREFIX}")
