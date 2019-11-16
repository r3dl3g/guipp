cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-core" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-core-obj)

    set(gui_include ${gui_include} ${CMAKE_CURRENT_BINARY_DIR})
    set(gui_include ${gui_include} PARENT_SCOPE)
    include_directories(${gui_include})

    file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

    message(STATUS "gui_defines: ${gui_defines}")

    if (WIN32)
      add_definitions(-DWIN32_LEAN_AND_MEAN)
    endif (WIN32)

    add_definitions(-DEXPERIMENTAL ${gui_defines})

    add_library(gui++-core-obj OBJECT ${SOURCE_FILES})

    set_target_properties(gui++-core-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if(GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_definitions(-DGUIPP_BUILT_AS_STATIC)
        add_library(gui++-core-static STATIC $<TARGET_OBJECTS:gui++-core-obj>)
        target_link_libraries(gui++-core-static PRIVATE ${logging_libraries})
    endif()

    if(GUIPP_BUILD_SHARED_MODULE_LIBS)
        add_definitions(-Dgui___core_obj_EXPORTS)
        add_library(gui++-core SHARED $<TARGET_OBJECTS:gui++-core-obj>)
        target_link_libraries(gui++-core PRIVATE ${logging_libraries} ${gui_sys_libraries} ${logging_sys_libraries})
        set_target_properties(gui++-core PROPERTIES
                                         POSITION_INDEPENDENT_CODE ON
                                         VERSION ${gui_VERSION}
                                         SOVERSION ${gui_VERSION_MAJOR})
    endif()

    GENERATE_EXPORT_HEADER(gui++-core-obj
        BASE_NAME GUIPP_CORE
        EXPORT_MACRO_NAME GUIPP_CORE_EXPORT
        EXPORT_FILE_NAME gui++-core-export.h
        STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

endif()
