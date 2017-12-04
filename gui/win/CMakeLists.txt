cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-win" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-win-obj)

    add_subdirectory(../core gui_core)

    include_directories(${gui_include})

    add_definitions(-DWIN32_LEAN_AND_MEAN -Dgui___EXPORTS)

    if(GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_definitions(-DGUIPP_BUILT_AS_STATIC)
    endif()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/Y-)
    endif()

    file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

    add_library(gui++-win-obj OBJECT ${SOURCE_FILES})
    set_target_properties(gui++-win-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if(GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_library(gui++-win-static STATIC $<TARGET_OBJECTS:gui++-win-obj>)
    endif()

    if(GUIPP_BUILD_SHARED_MODULE_LIBS)
        add_library(gui++-win SHARED $<TARGET_OBJECTS:gui++-win-obj>)

        target_link_libraries(gui++-win gui++-core ${gui_sys_libraries})

        set_target_properties(gui++-win PROPERTIES
                                        POSITION_INDEPENDENT_CODE ON
                                        VERSION ${gui_VERSION}
                                        SOVERSION ${gui_VERSION_MAJOR})
    endif()

endif()
