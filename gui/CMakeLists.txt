cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

set (gui_Include
	"${CMAKE_CURRENT_BINARY_DIR}" 
	"${CMAKE_CURRENT_SOURCE_DIR}/sources"
)
set (gui_Include ${gui_Include} PARENT_SCOPE)

if(NOT TARGET gui)

include_directories(${gui_Include})

set (gui_VERSION_MAJOR 0)
set (gui_VERSION_MINOR 0)
set (gui_VERSION_PATCH 1)
set (gui_VERSION "${gui_VERSION_MAJOR}.${gui_VERSION_MINOR}.${gui_VERSION_PATCH}")

# header file to pass version settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Version.h.in"
  "${PROJECT_BINARY_DIR}/gui_version.h"
)

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${gui_Lib})

add_library(gui ${SOURCE_FILES})

endif()
