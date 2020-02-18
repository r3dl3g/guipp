cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui-lib" CXX)

set(BUILD_FOR_ARM ON CACHE BOOL "On to build for arm architecture, Off for host architecture. default Off")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

function(print_var VAR)
    message(STATUS "${VAR}: '" ${${VAR}} "'")
endfunction(print_var)

print_var(CMAKE_CXX_PLATFORM_ID)
print_var(CMAKE_CXX_COMPILER_ID)
print_var(CMAKE_SYSTEM_PROCESSOR)

set (CMAKE_USE_RELATIVE_PATHS FALSE)

if (CMAKE_CXX_PLATFORM_ID STREQUAL "Windows" AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Building with Clang!")
    set(CMAKE_CXX_FLAGS "${_PLATFORM_DEFINES}${_PLATFORM_DEFINES_CXX} -fms-extensions -fms-compatibility -Fp -EHsc -D_WINDOWS")
    set(CMAKE_CXX_FLAGS_DEBUG  "/MDd -O0 -Zi")
    set(CMAKE_CXX_FLAGS_RELEASE  "/MD -O2 -DNDEBUG")
    set(INSTALL_BASE_DIR "C:\\Programm Files\\guipp")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
    add_compile_options(/Y-)
    set(INSTALL_BASE_DIR "C:\\Programm Files\\guipp")
elseif (CMAKE_CXX_PLATFORM_ID  STREQUAL "FreeBSD")
    set(CMAKE_CXX_FLAGS "-I/usr/local/include -I/usr/local/include/freetype2")
    link_directories(/usr/local/lib /usr/local/lib/gcc9)
    add_definitions(-DBSD)
    set(INSTALL_BASE_DIR "/usr/local")
else()
    set(INSTALL_BASE_DIR "/usr/local")
endif ()

add_definitions(-std=c++14)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX d)
endif ()

if (APPLE)
    add_definitions(-DCOCOA -lstdc++fs)
elseif (UNIX)
    add_definitions(-DX11 -lstdc++fs)
endif ()

if (BUILD_FOR_ARM)
    add_definitions(-DBUILD_FOR_ARM -Wno-psabi)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-std=c++14 -fexceptions -Wall -Wno-switch -Wno-unused-command-line-argument -Wno-sign-compare)
endif ()

add_subdirectory(gui)
add_subdirectory(persistent)
add_subdirectory(test)
add_subdirectory(RedImage)
add_subdirectory(ShowBtrfs)
add_subdirectory(TimeTracker)
if (BUILD_FOR_ARM)
    add_subdirectory(raspi)
endif ()
