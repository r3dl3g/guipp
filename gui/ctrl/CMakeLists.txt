cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-ctrl" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../win gui_win)
add_subdirectory(./../io gui_io)

if(NOT TARGET gui++-ctrl-obj)

    include_directories(${gui_Include})

    file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

    add_library(gui++-ctrl-obj OBJECT ${SOURCE_FILES})
    set_target_properties(gui++-ctrl-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if (GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_library(gui++-ctrl-static STATIC $<TARGET_OBJECTS:gui++-ctrl-obj>)
    endif(GUIPP_BUILD_STATIC_MODULE_LIBS)

    if (GUIPP_BUILD_SHARED_MODULE_LIBS)
        add_library(gui++-ctrl SHARED $<TARGET_OBJECTS:gui++-ctrl-obj>)

        target_link_libraries(gui++-ctrl gui++-core gui++-draw gui++-win gui++-io)

        set_target_properties(gui++-ctrl PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            VERSION ${gui_VERSION}
            SOVERSION ${gui_VERSION_MAJOR})
    endif(GUIPP_BUILD_SHARED_MODULE_LIBS)

endif()
