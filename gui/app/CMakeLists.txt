cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-app" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-app-obj)

    add_subdirectory(./../core gui_core)

    include_directories(${gui_include})

    file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

    if(GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_definitions(-DGUIPP_BUILT_AS_STATIC)
    endif()
    if (MSVC)
        add_compile_options(/Y-)
    endif()

    add_library(gui++-app-obj OBJECT ${SOURCE_FILES})
    set_target_properties(gui++-app-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

    add_library(gui++-app STATIC $<TARGET_OBJECTS:gui++-app-obj>)

endif()
