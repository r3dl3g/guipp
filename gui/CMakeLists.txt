cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

set (gui_Include
     ${CMAKE_CURRENT_SOURCE_DIR}/..
     ${CMAKE_CURRENT_BINARY_DIR}
)

if(UNIX)
  set (gui_Include ${gui_Include} /usr/include/freetype2)
endif(UNIX)

add_subdirectory(core gui_core)
add_subdirectory(draw gui_draw)
add_subdirectory(io gui_io)
add_subdirectory(win gui_win)
add_subdirectory(ctrl gui_ctrl)

set (gui_Include ${gui_Include} PARENT_SCOPE)

if(NOT TARGET gui)

set (gui_VERSION_MAJOR 0)
set (gui_VERSION_MINOR 1)
set (gui_VERSION_PATCH 0)
set (gui_VERSION "${gui_VERSION_MAJOR}.${gui_VERSION_MINOR}.${gui_VERSION_PATCH}")

# header file to pass version settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Version.h.in"
  "${PROJECT_BINARY_DIR}/gui_version.h"
)

endif()
