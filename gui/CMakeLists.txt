cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (GenerateExportHeader)

set(GUIPP_BUILD_STATIC_MODULE_LIBS OFF CACHE BOOL "On to build a static library for each submodule, default Off")
set(GUIPP_BUILD_SHARED_MODULE_LIBS OFF CACHE BOOL "On to build a shared library for each submodule, default Off")

set (gui_Include
     ${CMAKE_CURRENT_SOURCE_DIR}/..
     ${CMAKE_CURRENT_BINARY_DIR}
     ${PROJECT_BINARY_DIR}
)

if(UNIX)
  set (gui_Include ${gui_Include} /usr/include/freetype2)
endif(UNIX)

set (gui_VERSION_MAJOR 0)
set (gui_VERSION_MINOR 1)
set (gui_VERSION_PATCH 0)
set (gui_VERSION "${gui_VERSION_MAJOR}.${gui_VERSION_MINOR}.${gui_VERSION_PATCH}")

set (gui_Include ${gui_Include} PARENT_SCOPE)

add_subdirectory(core gui_core)
add_subdirectory(win gui_win)
add_subdirectory(draw gui_draw)
add_subdirectory(io gui_io)
add_subdirectory(ctrl gui_ctrl)
add_subdirectory(app gui_app)

if(NOT TARGET gui++)

    # header file to pass version settings to the source code
    configure_file (
        "${PROJECT_SOURCE_DIR}/Version.h.in"
        "${PROJECT_BINARY_DIR}/gui_version.h"
        )

    include_directories(${gui_Include} ${PROJECT_BINARY_DIR})

    add_library(gui++ SHARED gui.cpp
							 $<TARGET_OBJECTS:gui++-core-obj>
                             $<TARGET_OBJECTS:gui++-win-obj>
                             $<TARGET_OBJECTS:gui++-draw-obj>
                             $<TARGET_OBJECTS:gui++-io-obj>
                             $<TARGET_OBJECTS:gui++-ctrl-obj>)

    set_target_properties(gui++ PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        VERSION ${gui_VERSION}
        SOVERSION ${gui_VERSION_MAJOR})

endif()

GENERATE_EXPORT_HEADER(gui++
                       BASE_NAME GUIPP
                       EXPORT_MACRO_NAME GUIPP_EXPORT
                       EXPORT_FILE_NAME gui++-export.h
                       STATIC_DEFINE GUIPP_BUILT_AS_STATIC
                       )

