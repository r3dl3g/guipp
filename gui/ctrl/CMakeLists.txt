cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-ctrl" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-ctrl-obj)

    add_subdirectory(../win gui_win)
    add_subdirectory(../io gui_io)

    include_directories(${gui_include})

    add_definitions(-DWIN32_LEAN_AND_MEAN -Dgui___EXPORTS)

    if(GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_definitions(-DGUIPP_BUILT_AS_STATIC)
    endif()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/Y-)
    endif()

    file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

    add_library(gui++-ctrl-obj OBJECT ${SOURCE_FILES})

    set_target_properties(gui++-ctrl-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if(GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_library(gui++-ctrl-static STATIC $<TARGET_OBJECTS:gui++-ctrl-obj>)
    endif()

    if(GUIPP_BUILD_SHARED_MODULE_LIBS)
        add_library(gui++-ctrl SHARED $<TARGET_OBJECTS:gui++-ctrl-obj>)

        target_link_libraries(gui++-ctrl gui++-core gui++-draw gui++-win gui++-io ${gui_sys_libraries})

        set_target_properties(gui++-ctrl PROPERTIES
                                         POSITION_INDEPENDENT_CODE ON
                                         VERSION ${gui_VERSION}
                                         SOVERSION ${gui_VERSION_MAJOR})
    endif()

endif()
