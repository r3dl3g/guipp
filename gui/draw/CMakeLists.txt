cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-draw" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-draw-obj)

    add_subdirectory(../core gui_core)

    include_directories(${gui_include})

    add_definitions(-DWIN32_LEAN_AND_MEAN -Dgui___EXPORTS)

    if(GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_definitions(-DGUIPP_BUILT_AS_STATIC)
    endif()
    if (MSVC)
        add_compile_options(/Y-)
    endif()

    file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

    add_library(gui++-draw-obj OBJECT ${SOURCE_FILES})

    set_target_properties(gui++-draw-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if(GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_library(gui++-draw-static STATIC $<TARGET_OBJECTS:gui++-draw-obj>)
    endif()

    if(GUIPP_BUILD_SHARED_MODULE_LIBS)
        add_library(gui++-draw SHARED $<TARGET_OBJECTS:gui++-draw-obj>)

        target_link_libraries(gui++-draw gui++-core ${gui_sys_libraries})

        set_target_properties(gui++-draw PROPERTIES
                                         POSITION_INDEPENDENT_CODE ON
                                         VERSION ${gui_VERSION}
                                         SOVERSION ${gui_VERSION_MAJOR})
    endif()

endif()
