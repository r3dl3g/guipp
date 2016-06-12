cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("core" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

set (core_lib_Include
	"${CMAKE_CURRENT_BINARY_DIR}" 
	"${CMAKE_CURRENT_SOURCE_DIR}/sources"
)
set (core_Include ${core_Include} PARENT_SCOPE)

if(NOT TARGET core)

include_directories(${core_Include})

set (core_VERSION_MAJOR 0)
set (core_VERSION_MINOR 0)
set (core_VERSION_PATCH 1)
set (core_VERSION "${core_VERSION_MAJOR}.${core_VERSION_MINOR}.${core_VERSION_PATCH}")

# header file to pass version settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Version.h.in"
  "${PROJECT_BINARY_DIR}/core_version.h"
)

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${core_Lib})

add_library(core ${SOURCE_FILES})

endif()
