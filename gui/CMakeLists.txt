cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (GenerateExportHeader)

#unset(GUIPP_BUILD_STATIC_MODULE_LIBS CACHE)
#unset(GUIPP_BUILD_SHARED_MODULE_LIBS CACHE)

set(GUIPP_BUILD_STATIC_MODULE_LIBS OFF CACHE BOOL "On to build a static library for each submodule, Off to build shared libraries. Default Off")
set(GUIPP_BUILD_SHARED_MODULE_LIBS OFF CACHE BOOL "On to build a shared library for each submodule, Off to build static libraries. default Off")

if(NOT TARGET gui++ AND NOT TARGET gui++-static)

    set (gui_VERSION_MAJOR 0)
    set (gui_VERSION_MINOR 1)
    set (gui_VERSION_PATCH 0)
    set (gui_VERSION "${gui_VERSION_MAJOR}.${gui_VERSION_MINOR}.${gui_VERSION_PATCH}")

    add_subdirectory(../base base)

    message(STATUS "gui basepp_libraries: ${basepp_libraries}")
    message(STATUS "gui basepp_sys_libraries: ${basepp_sys_libraries}")

    if (WIN32)
      set (gui_defines ${basepp_defines} -DWIN32_LEAN_AND_MEAN)
    else ()
      set (gui_defines ${basepp_defines})
    endif ()

    set (gui_include
        ${basepp_include}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_BINARY_DIR}
    )

    if (APPLE)
        set (gui_defines ${gui_defines} -std=c++11)
    elseif (UNIX)
        set (gui_include ${gui_include} /usr/include/freetype2)
#        set (gui_sys_libraries X11 Xft fontconfig xcb Xdmcp bsd dl freetype Xrender expat Xau z png16)
        set (gui_sys_libraries X11 Xft fontconfig xcb Xdmcp freetype Xrender)
        set (gui_defines ${gui_defines} -DX11 -std=c++11)
    endif ()

    if (GUIPP_BUILD_STATIC_MODULE_LIBS)
        set (gui_defines ${gui_defines} -DGUIPP_BUILT_AS_STATIC)
        set (gui_libraries ${basepp_libraries}
                           gui++-ctrl-static
                           gui++-io-static
                           gui++-draw-static
                           gui++-win-static
                           gui++-core-static
                           gui++-app
                           gui++-static
        )
        set (gui_sys_libraries ${gui_sys_libraries} ${basepp_sys_libraries})
    elseif (GUIPP_BUILD_SHARED_MODULE_LIBS)
        set (gui_libraries gui++-ctrl
                           gui++-io
                           gui++-draw
                           gui++-win
                           gui++-core
                           gui++-app
        )
        set (gui_sys_libraries ${basepp_libraries} gui++ ${gui_sys_libraries} ${basepp_sys_libraries})
    else()
        set (gui_libraries gui++-app gui++)
        set (gui_sys_libraries ${basepp_libraries} ${gui_sys_libraries} ${basepp_sys_libraries})
    endif()

    if (NOT GUIPP_BUILD_STATIC_MODULE_LIBS AND NOT GUIPP_BUILD_SHARED_MODULE_LIBS)
        set (gui_defines ${gui_defines} -Dgui___core_obj_EXPORTS -Dgui___win_obj_EXPORTS -Dgui___draw_obj_EXPORTS -Dgui___io_obj_EXPORTS -Dgui___ctrl_obj_EXPORTS)
    endif()

    add_subdirectory(core gui_core)
    add_subdirectory(win gui_win)
    add_subdirectory(draw gui_draw)
    add_subdirectory(io gui_io)
    add_subdirectory(ctrl gui_ctrl)
    add_subdirectory(app gui_app)

    set (gui_defines ${gui_defines} PARENT_SCOPE)
    set (gui_include ${gui_include} PARENT_SCOPE)
    set (gui_libraries ${gui_libraries} PARENT_SCOPE)
    set (gui_sys_libraries ${gui_sys_libraries} PARENT_SCOPE)

    message(STATUS "gui gui_defines: ${gui_defines}")
    message(STATUS "gui gui_include: ${gui_include}")
    message(STATUS "gui gui_libraries: ${gui_libraries}")
    message(STATUS "gui gui_sys_libraries: ${gui_sys_libraries}")

    # header file to pass version settings to the source code
    configure_file (
        "${PROJECT_SOURCE_DIR}/Version.h.in"
        "${PROJECT_BINARY_DIR}/gui_version.h"
    )

    include_directories(${gui_include})

    if (NOT GUIPP_BUILD_STATIC_MODULE_LIBS AND NOT GUIPP_BUILD_SHARED_MODULE_LIBS)
        message(STATUS "gui build global shared gui library")

        add_definitions(-Dgui___EXPORTS ${gui_defines})

        add_library(gui++ SHARED $<TARGET_OBJECTS:gui++-core-obj>
                                 $<TARGET_OBJECTS:gui++-win-obj>
                                 $<TARGET_OBJECTS:gui++-draw-obj>
                                 $<TARGET_OBJECTS:gui++-io-obj>
                                 $<TARGET_OBJECTS:gui++-ctrl-obj>
                                 gui.cpp
        )

        target_link_libraries(gui++ PRIVATE ${gui_sys_libraries})

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

    elseif(GUIPP_BUILD_SHARED_MODULE_LIBS)
        message(STATUS "gui build multiple shared gui libraries")

        add_definitions(-Dgui___EXPORTS ${gui_defines})
        
        add_library(gui++ SHARED gui.cpp)

        target_link_libraries(gui++ PRIVATE ${basepp_libraries} ${gui_sys_libraries} ${basepp_sys_libraries})

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

    else()
        message(STATUS "gui build multiple static gui libraries")

        add_definitions(${gui_defines})
        
        add_library(gui++-static STATIC gui.cpp)

        set_target_properties(gui++-static PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            VERSION ${gui_VERSION}
            SOVERSION ${gui_VERSION_MAJOR}
        )

        GENERATE_EXPORT_HEADER(gui++-static
            BASE_NAME GUIPP
            EXPORT_MACRO_NAME GUIPP_EXPORT
            EXPORT_FILE_NAME gui++-export.h
            STATIC_DEFINE GUIPP_BUILT_AS_STATIC
        )

    endif()
    
endif()
