cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-io" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../draw gui_draw)

if(NOT TARGET gui++-io-obj)

    include_directories(${gui_Include})

    add_definitions(-DWIN32_LEAN_AND_MEAN -Dgui___EXPORTS)
	if(CMAKE_COMPILER_IS_CLANG)
		add_definitions(-Wno-inconsistent-dllimport)
	endif()

    file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

    add_library(gui++-io-obj OBJECT ${SOURCE_FILES})
    set_target_properties(gui++-io-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if (GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_library(gui++-io-static STATIC $<TARGET_OBJECTS:gui++-io-obj>)
    endif(GUIPP_BUILD_STATIC_MODULE_LIBS)

    if (GUIPP_BUILD_SHARED_MODULE_LIBS)
        add_library(gui++-io SHARED $<TARGET_OBJECTS:gui++-io-obj>)

        target_link_libraries(gui++-io gui++-draw gui++-core)

        set_target_properties(gui++-io PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            VERSION ${gui_VERSION}
            SOVERSION ${gui_VERSION_MAJOR})
    endif(GUIPP_BUILD_SHARED_MODULE_LIBS)

endif()
