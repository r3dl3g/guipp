cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (GenerateExportHeader)

set(GUIPP_BUILD_STATIC_MODULE_LIBS OFF CACHE BOOL "On to build a static library for each submodule, default Off")
set(GUIPP_BUILD_SHARED_MODULE_LIBS OFF CACHE BOOL "On to build a shared library for each submodule, default Off")

if(NOT TARGET gui++)

    add_subdirectory(../base base)

    set (gui_include
        ${basepp_include}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_BINARY_DIR}
    )

    if(UNIX)
        set (gui_include ${gui_include} /usr/include/freetype2)
        set (gui_sys_libraries X11 Xft fontconfig)
    endif(UNIX)

    if (GUIPP_BUILD_STATIC_MODULE_LIBS)
        set (gui_libraries gui++-ctrl-static gui++-io-static gui++-draw-static gui++-win-static gui++-core-static gui++-app)
    elseif (GUIPP_BUILD_SHARED_MODULE_LIBS)
        set (gui_libraries gui++-ctrl gui++-app)
    else()
        set (gui_libraries gui++ gui++-app)
    endif()

    message(STATUS "basepp_libraries: ${basepp_libraries}")

    set (gui_libraries ${gui_libraries} ${basepp_libraries} ${gui_sys_libraries})

    set (gui_include ${gui_include} PARENT_SCOPE)
    set (gui_libraries ${gui_libraries} PARENT_SCOPE)
    set (gui_sys_libraries ${gui_sys_libraries} PARENT_SCOPE)

    set (gui_VERSION_MAJOR 0)
    set (gui_VERSION_MINOR 1)
    set (gui_VERSION_PATCH 0)
    set (gui_VERSION "${gui_VERSION_MAJOR}.${gui_VERSION_MINOR}.${gui_VERSION_PATCH}")

    add_subdirectory(core gui_core)
    add_subdirectory(win gui_win)
    add_subdirectory(draw gui_draw)
    add_subdirectory(io gui_io)
    add_subdirectory(ctrl gui_ctrl)
    add_subdirectory(app gui_app)

    # header file to pass version settings to the source code
    configure_file (
        "${PROJECT_SOURCE_DIR}/Version.h.in"
        "${PROJECT_BINARY_DIR}/gui_version.h"
    )

    include_directories(${gui_include})

    if (NOT GUIPP_BUILD_STATIC_MODULE_LIBS AND NOT GUIPP_BUILD_SHARED_MODULE_LIBS)

        add_library(gui++ SHARED $<TARGET_OBJECTS:gui++-core-obj>
                                 $<TARGET_OBJECTS:gui++-win-obj>
                                 $<TARGET_OBJECTS:gui++-draw-obj>
                                 $<TARGET_OBJECTS:gui++-io-obj>
                                 $<TARGET_OBJECTS:gui++-ctrl-obj>
        )

        set_target_properties(gui++ PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            VERSION ${gui_VERSION}
            SOVERSION ${gui_VERSION_MAJOR}
        )

        GENERATE_EXPORT_HEADER(gui++
            BASE_NAME GUIPP
            EXPORT_MACRO_NAME GUIPP_EXPORT
            EXPORT_FILE_NAME gui++-export.h
            STATIC_DEFINE GUIPP_BUILT_AS_STATIC
        )

    endif()

endif()

