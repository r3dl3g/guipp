cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui-lib" CXX)

if (CMAKE_CXX_PLATFORM_ID STREQUAL "Windows" AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	message(STATUS "Building with Clang!")
        set(CMAKE_CXX_FLAGS "${_PLATFORM_DEFINES}${_PLATFORM_DEFINES_CXX} -fms-extensions -fms-compatibility -Fp -EHsc -D_WINDOWS")
	set(CMAKE_CXX_FLAGS_DEBUG  "/MDd -O0 -Zi")
	set(CMAKE_CXX_FLAGS_RELEASE  "/MD -O2 -DNDEBUG")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
	add_compile_options(/Y-)
endif ()

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif ()

if (APPLE)
  add_definitions(-DCOCOA -std=c++11 -lstdc++fs)
elseif (UNIX)
  add_definitions(-DX11 -std=c++11 -lstdc++fs)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_definitions(-std=c++14 -fexceptions -Wall -Wno-switch -Wno-unused-command-line-argument -Wno-sign-compare )
endif ()

add_subdirectory(base)
add_subdirectory(gui)
add_subdirectory(test)
add_subdirectory(TimeTracker)
