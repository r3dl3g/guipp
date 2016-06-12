cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("draw" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

set (draw_Include
	${core_Include}
	"${CMAKE_CURRENT_BINARY_DIR}" 
	"${CMAKE_CURRENT_SOURCE_DIR}/sources"
)
set (draw_Include ${draw_Include} PARENT_SCOPE)

if(NOT TARGET draw)

add_subdirectory(./../core core)

include_directories(${draw_Include})

set (draw_VERSION_MAJOR 0)
set (draw_VERSION_MINOR 0)
set (draw_VERSION_PATCH 1)
set (draw_VERSION "${draw_VERSION_MAJOR}.${draw_VERSION_MINOR}.${draw_VERSION_PATCH}")

# header file to pass version settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Version.h.in"
  "${PROJECT_BINARY_DIR}/draw_version.h"
)

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${draw_Lib})

add_library(draw ${SOURCE_FILES})

endif()
