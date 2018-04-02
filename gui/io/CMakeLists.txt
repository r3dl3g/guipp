cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-io" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-io-obj)

    add_subdirectory(../draw gui_draw)

    set(gui_include ${gui_include} ${CMAKE_CURRENT_BINARY_DIR})
    set(gui_include ${gui_include} PARENT_SCOPE)
    include_directories(${gui_include})

    if (WIN32)
      add_definitions(-DWIN32_LEAN_AND_MEAN)
    endif (WIN32)

    add_definitions(${gui_defines})

    file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

    add_library(gui++-io-obj OBJECT ${SOURCE_FILES})
    set_target_properties(gui++-io-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if(GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_definitions(-DGUIPP_BUILT_AS_STATIC)
        add_library(gui++-io-static STATIC $<TARGET_OBJECTS:gui++-io-obj>)
    endif()

    if(GUIPP_BUILD_SHARED_MODULE_LIBS)
        add_definitions(-Dgui___io_obj_EXPORTS)
        add_library(gui++-io SHARED $<TARGET_OBJECTS:gui++-io-obj>)
        target_link_libraries(gui++-io PRIVATE gui++-draw gui++-core ${gui_sys_libraries})
        set_target_properties(gui++-io PROPERTIES
                                       POSITION_INDEPENDENT_CODE ON
                                       VERSION ${gui_VERSION}
                                       SOVERSION ${gui_VERSION_MAJOR})
    endif()

    GENERATE_EXPORT_HEADER(gui++-io-obj
        BASE_NAME GUIPP_IO
        EXPORT_MACRO_NAME GUIPP_IO_EXPORT
        EXPORT_FILE_NAME gui++-io-export.h
        STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

endif()
