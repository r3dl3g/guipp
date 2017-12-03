cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-core" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-core-obj)

    include_directories(${gui_Include})

    file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

    add_definitions(-DEXPERIMENTAL -DWIN32_LEAN_AND_MEAN -Dgui___EXPORTS)

    add_library(gui++-core-obj OBJECT ${SOURCE_FILES})
    set_target_properties(gui++-core-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if (GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_library(gui++-core-static STATIC $<TARGET_OBJECTS:gui++-core-obj>)
    endif(GUIPP_BUILD_STATIC_MODULE_LIBS)

    if (GUIPP_BUILD_SHARED_MODULE_LIBS)
        add_library(gui++-core SHARED $<TARGET_OBJECTS:gui++-core-obj>)

        set_target_properties(gui++-core PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            VERSION ${gui_VERSION}
            SOVERSION ${gui_VERSION_MAJOR})
    endif(GUIPP_BUILD_SHARED_MODULE_LIBS)

endif()
