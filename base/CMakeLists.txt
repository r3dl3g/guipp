cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("base++" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (GenerateExportHeader)

unset(BASEPP_BUILD_STATIC_MODULE_LIBS CACHE)
set(BASEPP_BUILD_STATIC_MODULE_LIBS ON CACHE BOOL "On to build a static library for each submodule, Off for shared library. default On")

if(NOT TARGET base++-obj)

    set (basepp_include
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_BINARY_DIR}
    )

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set (basepp_sys_libraries stdc++fs pthread)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (basepp_sys_libraries stdc++fs pthread)
    endif()

    if (BASEPP_BUILD_STATIC_MODULE_LIBS)
        set (basepp_libraries base++-static)
        set (basepp_defines "-DBASEPP_BUILT_AS_STATIC")
    else()
        set (basepp_libraries base++)
    endif()
    set (basepp_libraries ${basepp_libraries} ${basepp_sys_libraries})

    set (basepp_sys_libraries ${basepp_sys_libraries} PARENT_SCOPE)
    set (basepp_include ${basepp_include} PARENT_SCOPE)
    set (basepp_libraries ${basepp_libraries} PARENT_SCOPE)
    set (basepp_defines ${basepp_defines} PARENT_SCOPE)

    message(STATUS "basepp_defines: ${basepp_defines}")

    set (BASEPP_VERSION_MAJOR 0)
    set (BASEPP_VERSION_MINOR 1)
    set (BASEPP_VERSION_PATCH 0)
    set (BASEPP_VERSION "${BASEPP_VERSION_MAJOR}.${BASEPP_VERSION_MINOR}.${BASEPP_VERSION_PATCH}")

    include_directories(${basepp_include})

    # header file to pass version settings to the source code
    configure_file (
        "${PROJECT_SOURCE_DIR}/version.h.in"
        "${PROJECT_BINARY_DIR}/base_version.h"
    )

    file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

    add_definitions(-DEXPERIMENTAL -DWIN32_LEAN_AND_MEAN -Dbase___obj_EXPORTS ${basepp_defines})

    add_library(base++-obj OBJECT ${SOURCE_FILES})

    set_target_properties(base++-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if(BASEPP_BUILD_STATIC_MODULE_LIBS)
        add_library(base++-static STATIC $<TARGET_OBJECTS:base++-obj>)
    else()
        add_library(base++ SHARED $<TARGET_OBJECTS:base++-obj>)

        target_link_libraries(base++ PRIVATE ${basepp_sys_libraries})

        set_target_properties(base++ PROPERTIES
                                     POSITION_INDEPENDENT_CODE ON
                                     VERSION ${BASEPP_VERSION}
                                     SOVERSION ${BASEPP_VERSION_MAJOR})
    endif()

    GENERATE_EXPORT_HEADER(base++-obj
        BASE_NAME BASEPP
        EXPORT_MACRO_NAME BASEPP_EXPORT
        EXPORT_FILE_NAME base++-export.h
        STATIC_DEFINE BASEPP_BUILT_AS_STATIC
    )

endif()
