cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

file(READ "../derivation.nix" derivation_ver)
string(REGEX MATCH "version = \"([0-9\.]+)\"" _ ${derivation_ver})
set(project_version ${CMAKE_MATCH_1})

message("Building gui version: ${project_version}")

project("gui" LANGUAGES CXX VERSION ${project_version})
enable_language(CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (Functions.cmake)
include (../Functions.cmake)

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

if(POLICY CMP0177)
  cmake_policy(SET CMP0177 NEW)
endif()

if(NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
    NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif()

set(GUIPP_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(GUIPP_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(GUIPP_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(GUIPP_VERSION "${PROJECT_VERSION}")

if(NOT "${CMAKE_CXX_STANDARD}")
  set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard")
endif()

# Generale build setting
option(GUIPP_BUILD_STATIC_MODULE_LIBS "On to build a static library for each submodule, Off to build shared libraries. Default Off" OFF)
option(GUIPP_BUILD_SHARED_MODULE_LIBS "On to build a shared library for each submodule, Off to build static libraries. default Off" ON)
option(GUIPP_CONFIG_INSTALL "On to configure installation, Off to build as part of a project. Default Off" ON)
option(GUIPP_BUILD_DEPENDENT_LIBS "On to build dependent libs logging, util and testlib.  Default Off" OFF)
# Front-end Settings
option(GUIPP_USE_QT5 "On to make use of the Qt5 library. Default Off" OFF)
option(GUIPP_USE_QT6 "On to make use of the Qt6 library. Default Off" OFF)
option(GUIPP_USE_SDL "On to make use of the SDL library. Default Off" OFF)
option(GUIPP_USE_X11 "On to make use of the X11 library. Default on Linux" OFF)
option(GUIPP_USE_GDI "On to make use of the GDI32 library. Default on Windows" OFF)
option(GUIPP_USE_JS  "On to make use of the Emscripten-JS Bridge. Default for Emscripten" OFF)
# X11 Front-end specific settings
option(GUIPP_USE_XCB "On to make use of the xcb library. Default On" ON)
option(GUIPP_DONT_USE_XCB "On to NOT use of the xcb library. Default Off" OFF)
option(GUIPP_USE_XSHM "On to make use of the xshm library. Default On" ON)
option(GUIPP_DONT_USE_XSHM "On to NOT use of the xshm library. Default Off" OFF)
option(GUIPP_DONT_USE_XFT "On to NOT use of the xft library. Default Off" OFF)
# Filesystem specific settings
option(GUIPP_USE_BOOST "On to use boost library for filesystem. Default Off" OFF)

# Look'n Feel Settings
option(GUIPP_BUILD_FOR_MOBILE "On to optimize for smartphone, Off to optimize for dekstop. Default Off" OFF)
option(GUIPP_LOOK_AND_FEEL_APPLE "Use Apple L&F. Default automatic" OFF)
option(GUIPP_LOOK_AND_FEEL_WIN32 "Use Windows 10 L&F. Default automatic" OFF)
option(GUIPP_LOOK_AND_FEEL_UNIX "Use Gnome L&F. Default automatic" OFF)
option(GUIPP_LOOK_AND_FEEL_W95 "Use Windows 95 L&F. Default automatic" OFF)
option(GUIPP_POPUP_OVERLAPP "Show Popupmenus and Dialogs as floating overlapped windows. Default automatic" ON)
# Target Systems (some set by cmake)
# Linux: IF (UNIX)
# Windows: IF (WIN32)

# Windows via mingw: IF (MINGW)
if(CMAKE_CXX_PLATFORM_ID STREQUAL MinGW)
  # For older cmkae versions
  set(MINGW 1)
endif()

# Emscripten: IF (EMSCRIPTEN)
if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")
  set(EMSCRIPTEN 1)
endif()

# Android: IF (ANDROID)
# Apple: IF (APPLE)

option(VERBOSE "Print messages during cmake run" OFF)
set(GUIPP_CXX_STANDARD ${CMAKE_CXX_STANDARD} CACHE STRING "C++ standard to overwrite default cmake standard")
if(NOT "${GUIPP_CXX_STANDARD}")
  set(GUIPP_CXX_STANDARD ${CMAKE_CXX_STANDARD} CACHE STRING "C++ standard")
endif()
set(LOGGING_CXX_STANDARD "${GUIPP_CXX_STANDARD}" CACHE STRING "C++ standard")
set(UTIL_CXX_STANDARD "${GUIPP_CXX_STANDARD}" CACHE STRING "C++ standard")
set(TESTING_CXX_STANDARD "${GUIPP_CXX_STANDARD}" CACHE STRING "C++ standard")
set(GUIPP_LIBS_DIR "${PROJECT_SOURCE_DIR}/libs" CACHE STRING "Path to required libraries")

guipp_target_suffix(TARGET_SUFFIX)

debug_print_var(CMAKE_SOURCE_DIR)
debug_print_var(CMAKE_CURRENT_SOURCE_DIR)
debug_print_var(CMAKE_ROOT_SOURCE_DIR)
debug_print_var(CMAKE_BINARY_DIR)
debug_print_var(CMAKE_CURRENT_BINARY_DIR)
debug_print_var(CMAKE_ROOT_BINARY_DIR)
debug_print_var(CMAKE_PREFIX_PATH)
debug_print_var(CMAKE_FIND_ROOT_PATH)
debug_print_var(CMAKE_INSTALL_LIBDIR)
debug_print_var(TARGET_SUFFIX)

if(GUIPP_BUILD_STATIC_MODULE_LIBS)
  set(UTIL_BUILD_STATIC_MODULE_LIB ON CACHE BOOL "enable static build")
  set(LOGGING_BUILD_STATIC_MODULE_LIB ON CACHE BOOL "enable static build")
elseif(GUIPP_BUILD_SHARED_MODULE_LIBS)
  set(UTIL_BUILD_STATIC_MODULE_LIB OFF CACHE BOOL "disable static build")
  set(LOGGING_BUILD_STATIC_MODULE_LIB OFF CACHE BOOL "disable static build")
endif()

if(GUIPP_BUILD_SHARED_MODULE_LIBS OR GUIPP_BUILD_STATIC_MODULE_LIBS)
  set(UTIL_CONFIG_INSTALL ON CACHE BOOL "enable install")
  set(LOGGING_CONFIG_INSTALL ON CACHE BOOL "enable install")
  set(TESTING_CONFIG_INSTALL ON CACHE BOOL "enable install")
endif()

if(EMSCRIPTEN)
  # Emscripten dos not search on CMAKE_PREFIX_PATH !(?)
  set(logging_DIR ${CMAKE_PREFIX_PATH}/lib/cmake/logging)
  set(util_DIR ${CMAKE_PREFIX_PATH}/lib/cmake/util)
endif()

if(GUIPP_BUILD_DEPENDENT_LIBS)
  add_subdirectory(${GUIPP_LIBS_DIR}/logging logging)
  add_subdirectory(${GUIPP_LIBS_DIR}/util util)
else ()
  find_package(logging REQUIRED VERSION 1.1)
  find_package(util REQUIRED VERSION 1.5)
endif()

debug_print_var(LOGGING_INCLUDE_DIRS)
debug_print_var(UTIL_INCLUDE_DIRS)

if(NOT TARGET guipp)

  #
  # SEttings inheritance
  #
  set(GUIPP_INCLUDE_DIRS
    ${LOGGING_INCLUDE_DIRS}
    ${UTIL_INCLUDE_DIRS}
  )

  set(LINKED_LIBRARIES
    ${LOGGING_LIBRARIES}
    ${UTIL_LIBRARIES}
  )

  set(GUIPP_CXX_FLAGS ${LOGGING_CXX_FLAGS} ${UTIL_CXX_FLAGS})
  set(GUIPP_EXE_LINKER_FLAGS "")

  #
  # Target system general
  #

  if(MINGW)
    MessagePrint("GUIPP: build with MinGW for Win32!")

    if (GUIPP_USE_X11 OR GUIPP_USE_JS)
      message(FATAL_ERROR "Unsupportet front-end for MinGW!")
    elseif (NOT GUIPP_USE_QT5 AND NOT GUIPP_USE_QT6 AND NOT GUIPP_USE_SDL)
      # Default GDI system
      set(GUIPP_USE_GDI 1)
    endif()

    set(GUIPP_BUILD_STATIC_MODULE_LIBS ON)
    set(GUIPP_BUILD_SHARED_MODULE_LIBS OFF)
    set(GUIPP_LINK_OPTIIONS PRIVATE "-Wl,-u,WinMain")

    list(APPEND GUIPP_CXX_FLAGS -DUSE_MINGW -D_WIN32_WINNT=0x0601 -DWIN32_LEAN_AND_MEAN -D_USE_MATH_DEFINES)
    list(APPEND GUIPP_EXE_LINKER_FLAGS -static-libgcc -static-libstdc++ -static)
    list(APPEND GUIPP_SYS_LIBRARIES mingw32 gdi32 user32 msimg32)

  elseif(WIN32)
    MessagePrint("GUIPP: build for Windows")

    if (GUIPP_USE_X11 OR GUIPP_USE_JS)
      message(FATAL_ERROR "Unsupportet front-end for MinGW!")
    elseif (NOT GUIPP_USE_QT5 AND NOT GUIPP_USE_QT6 AND NOT GUIPP_USE_SDL)
      # Default GDI system
      set(GUIPP_USE_GDI 1)
    endif()

    find_library(GDI32_LIB gdi32 REQUIRED)
    find_library(USER32_LIB user32 REQUIRED)
    find_library(MSIMG32_LIB msimg32 REQUIRED)

    list(APPEND GUIPP_CXX_FLAGS -DWIN32_LEAN_AND_MEAN -D_USE_MATH_DEFINES)
    list(APPEND LINKED_LIBRARIES ${GDI32_LIB} ${USER32_LIB} ${MSIMG32_LIB})
    list(APPEND GUIPP_SYS_LIBRARIES Shcore)


  elseif(APPLE)
    MessagePrint("GUIPP: build for macOS")

    if (GUIPP_USE_X11 OR GUIPP_USE_JS OR GUIPP_USE_GDI)
      message(FATAL_ERROR "Unsupportet front-end for macOS!")
    elseif (NOT GUIPP_USE_QT5 AND NOT GUIPP_USE_QT6)
      # Default GDI system
      set(GUIPP_USE_SDL 1)
    endif()

    list(APPEND GUIPP_CXX_FLAGS -DCOCOA -fPIC)

  elseif(ANDROID)
    MessagePrint("GUIPP: build for Android")

    if (GUIPP_USE_X11 OR GUIPP_USE_JS OR GUIPP_USE_GDI)
      message(FATAL_ERROR "Unsupportet front-end for Android!")
    elseif (NOT GUIPP_USE_QT5 AND NOT GUIPP_USE_QT6)
      # Default GDI system
      set(GUIPP_USE_SDL 1)
    endif()

    list(APPEND GUIPP_CXX_FLAGS -DANDROID)

    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} log)

  elseif(EMSCRIPTEN)
    MessagePrint("GUIPP: build for Emscripten")

    if (GUIPP_USE_X11 OR GUIPP_USE_GDI OR GUIPP_USE_QT5 OR GUIPP_USE_QT6)
      message(FATAL_ERROR "Unsupportet front-end for Android!")
    elseif (NOT GUIPP_USE_SDL)
      # Default GDI system
      set(GUIPP_USE_JS 1)
    endif()

    set(GUIPP_BUILD_STATIC_MODULE_LIBS ON)
    set(GUIPP_BUILD_SHARED_MODULE_LIBS OFF)
    set(GUIPP_POPUP_OVERLAPP OFF)

    list(APPEND GUIPP_CXX_FLAGS -Wno-unknown-pragmas)
    list(APPEND GUIPP_EXE_LINKER_FLAGS --bind
      -sNO_DISABLE_EXCEPTION_CATCHING
      -sDYNAMIC_EXECUTION=0
      -Wno-limited-postlink-optimizations
      -Wno-pthreads-mem-growth
    )

  elseif(UNIX)
    MessagePrint("GUIPP: build for Unix!")

    if (GUIPP_USE_JS OR GUIPP_USE_GDI)
      message(FATAL_ERROR "Unsupportet front-end for Unix!")
    elseif (NOT GUIPP_USE_SDL AND NOT GUIPP_USE_QT5 AND NOT GUIPP_USE_QT6)
      # Default GDI system
      set(GUIPP_USE_X11 1)
    endif()

    list(APPEND GUIPP_CXX_FLAGS -fPIC)

  else()
    message(SEND_ERROR "Undefined target system!")
  endif()

  #
  # Filesystem specific settings
  #

  if(GUIPP_USE_BOOST)
    find_package(Boost 1.65 REQUIRED COMPONENTS filesystem)
    list(APPEND GUIPP_CXX_FLAGS -DUSE_BOOST)
    if(NOT WIN32)
      list(APPEND GUIPP_SYS_LIBRARIES dl)
    endif()
  endif()

  #
  # Look'n Feel Settings
  #

  if(GUIPP_BUILD_FOR_MOBILE)
    list(APPEND GUIPP_CXX_FLAGS -DGUIPP_BUILD_FOR_MOBILE)
  endif()

  if(GUIPP_LOOK_AND_FEEL_APPLE)
    list(APPEND GUIPP_CXX_FLAGS -DGUIPP_LOOK_AND_FEEL_APPLE)
  elseif(GUIPP_LOOK_AND_FEEL_WIN32)
    list(APPEND GUIPP_CXX_FLAGS -DGUIPP_LOOK_AND_FEEL_WIN32)
  elseif(GUIPP_LOOK_AND_FEEL_UNIX)
    list(APPEND GUIPP_CXX_FLAGS -DGUIPP_LOOK_AND_FEEL_UNIX)
  elseif(GUIPP_LOOK_AND_FEEL_W95)
    list(APPEND GUIPP_CXX_FLAGS -DGUIPP_LOOK_AND_FEEL_W95)
  endif()

  #
  # Front-end Settings
  #

  if (GUIPP_USE_SDL)
    MessagePrint("GUIPP: build with SDL")

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    pkg_check_modules(SDL2_GFX REQUIRED SDL2_gfx)

    set(GUIPP_POPUP_OVERLAPP OFF)
    list(APPEND GUIPP_INCLUDE_DIRS
      ${SDL2_INCLUDE_DIRS} 
      ${SDL2_TTF_INCLUDE_DIRS}
      ${SDL2_GFX_INCLUDE_DIRS}
      "../libs/sdl_font_cache"
    )
    list(APPEND GUIPP_CXX_FLAGS
        -DGUIPP_SDL
        -DGUIPP_LOOK_AND_FEEL_WIN32
        ${SDL2_LDFLAGS}
        ${SDL2_TTF_LDFLAGS})

    list(APPEND GUIPP_SYS_LIBRARIES ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_GFX_LIBRARIES})

  elseif(GUIPP_USE_QT6)
    MessagePrint("GUIPP: build with Qt6")

    find_package(Qt6Core)
    find_package(Qt6Gui)

    list(APPEND LINKED_LIBRARIES Qt6Gui)

    list(APPEND GUIPP_SYS_LIBRARIES ${Qt6Gui_LIBRARIES})
    list(APPEND GUIPP_CXX_FLAGS -DGUIPP_QT ${Qt6Gui_DEFINITIONS})

    if(UNIX)
      list(APPEND GUIPP_SYS_LIBRARIES stdc++fs)
    endif()

    list(APPEND GUIPP_INCLUDE_DIRS ${Qt6Gui_INCLUDE_DIRS})

    DebugPrint("GUIPP: Qt6Gui_DEFINITIONS: " ${Qt6Gui_DEFINITIONS})
    DebugPrint("GUIPP: Qt6Gui_INCLUDE_DIRS: " ${Qt6Gui_INCLUDE_DIRS})
    DebugPrint("GUIPP: Qt6Gui_LIBRARIES: " ${Qt6Gui_LIBRARIES})

  elseif(GUIPP_USE_QT5)
    MessagePrint("GUIPP: build with Qt5")

    find_package(Qt5Gui REQUIRED)

    list(APPEND LINKED_LIBRARIES Qt5Gui)

    set(GUIPP_POPUP_OVERLAPP OFF)
    list(APPEND GUIPP_SYS_LIBRARIES ${Qt5Gui_LIBRARIES})
    list(APPEND GUIPP_CXX_FLAGS -DGUIPP_QT ${Qt5Gui_DEFINITIONS})

    if(UNIX)
      list(APPEND GUIPP_SYS_LIBRARIES stdc++fs)
    endif()

    list(APPEND GUIPP_INCLUDE_DIRS ${Qt5Gui_INCLUDE_DIRS})

    DebugPrint("GUIPP: Qt5Gui_DEFINITIONS: " ${Qt5Gui_DEFINITIONS})
    DebugPrint("GUIPP: Qt5Gui_INCLUDE_DIRS: " ${Qt5Gui_INCLUDE_DIRS})
    DebugPrint("GUIPP: Qt5Gui_LIBRARIES: " ${Qt5Gui_LIBRARIES})

  elseif(GUIPP_USE_JS)
    MessagePrint("GUIPP: build with JS")

    list(APPEND GUIPP_CXX_FLAGS
      -DGUIPP_JS
      -DGUIPP_LOOK_AND_FEEL_WIN32
      -sENVIRONMENT=worker
      -sDYNAMIC_EXECUTION=0
      -matomics
    )

    list(APPEND GUIPP_EXE_LINKER_FLAGS 
      -sENVIRONMENT=worker
      -sASYNCIFY=1
      -sASYNCIFY_STACK_SIZE=65536
      -sALLOW_MEMORY_GROWTH=1
      -sMODULARIZE=1
      -sEXPORT_NAME='createModule'
    )

  elseif(GUIPP_USE_GDI)
    MessagePrint("GUIPP: build with MinGW for Gdi32")

    list(APPEND GUIPP_CXX_FLAGS -mwindows -DGUIPP_WIN)

  elseif(GUIPP_USE_X11)
    MessagePrint("GUIPP: build for X11!")
    find_package(X11 REQUIRED)

    if(X11_FOUND)
      DebugPrint("GUIPP: X11 found!")
      if(X11_Xdmcp_FOUND)
        DebugPrint("GUIPP: X11_Xdmcp found!")
      else()
        message(SEND_ERROR "Xdmcp library is required!")
      endif()
      list(APPEND GUIPP_SYS_LIBRARIES ${X11_LIBRARIES} ${X11_Xdmcp_LIB} stdc++fs)
      list(APPEND GUIPP_CXX_FLAGS -DGUIPP_X11)
      list(APPEND GUIPP_INCLUDE_DIRS ${X11_INCLUDE_DIR})

      if(X11_Xrender_FOUND)
        DebugPrint("GUIPP: X11_Xrender found!")
      endif()

      find_package(Freetype)
      if(Freetype_FOUND)
        DebugPrint("GUIPP: Freetype found!")
      endif()

      if (GUIPP_DONT_USE_XFT)
        DebugPrint("GUIPP: Don't use Xft!")
      elseif (X11_Xft_FOUND)

        DebugPrint("GUIPP: X11_Xft found!")
        if(NOT X11_Xrender_FOUND)
          message(SEND_ERROR "Xrender library is required!")
        endif()
        if(NOT Freetype_FOUND)
          message(SEND_ERROR "Freetype library is required!")
        endif()

        find_package(Fontconfig)

        if(NOT Fontconfig_FOUND)
          message(SEND_ERROR "Fontconfig library is required!")
        endif()

        list(APPEND GUIPP_SYS_LIBRARIES ${X11_Xft_LIB} ${X11_Xrender_LIB} ${FREETYPE_LIBRARIES} ${Fontconfig_LIBRARIES})
        list(APPEND GUIPP_CXX_FLAGS -DGUIPP_USE_XFT)
        list(APPEND GUIPP_INCLUDE_DIRS ${X11_Xft_INCLUDE_PATH} ${X11_Xrender_INCLUDE_PATH} ${FREETYPE_INCLUDE_DIRS})

      else()
        DebugPrint("GUIPP: X11_Xft NOT found!")
      endif()

      if(X11_Xrandr_FOUND)
        DebugPrint("GUIPP: X11_Xrandr found!")
        list(APPEND GUIPP_CXX_FLAGS -DGUIPP_USE_XRANDR)
        list(APPEND GUIPP_SYS_LIBRARIES ${X11_Xrandr_LIB})
      endif()

      if (GUIPP_DONT_USE_XCB)
        DebugPrint("GUIPP: Don't use xcb!")
      else ()
        if(X11_X11_xcb_FOUND)
          DebugPrint("GUIPP: xcb found!")
          set(GUIPP_USE_XCB ON)
        endif()

        if(GUIPP_USE_XCB)
          list(APPEND GUIPP_SYS_LIBRARIES X11-xcb xcb xcb-xrm)
          list(APPEND GUIPP_CXX_FLAGS -DGUIPP_USE_XCB)
        endif()
      endif()

      if (GUIPP_DONT_USE_XSHM)
        DebugPrint("GUIPP: Don't use XShm!")
      else()
        find_library(XEXT_LIB Xext)

        if(XEXT_LIB)
          DebugPrint("GUIPP: xext found!")
          set(GUIPP_USE_XSHM ON)
        endif()

        if(GUIPP_USE_XSHM)
          list(APPEND GUIPP_SYS_LIBRARIES ${XEXT_LIB})
          list(APPEND GUIPP_CXX_FLAGS -DGUIPP_USE_XSHM)
        endif()
      endif()

    else()
      message(SEND_ERROR "X11 libraries are required!")
    endif(X11_FOUND)
    #        set(GUIPP_SYS_LIBRARIES X11 Xft fontconfig xcb Xdmcp bsd dl freetype Xrender expat Xau z png16)

  else()
    message(FATAL_ERROR "Undefined Front-end!")
  endif()

  if (GUIPP_POPUP_OVERLAPP)
    DebugPrint("GUIPP: Use overlapped popups!")

    list(APPEND GUIPP_CXX_FLAGS -DGUIPP_POPUP_OVERLAPP)
  endif()

  if(MSVC)
    DebugPrint("GUIPP: Use msvc compiler")

    list(APPEND GUIPP_EXE_LINKER_FLAGS /Y- /bigobj)
    set(GUIPP_LINK_PROPERTIES LINK_FLAGS "/INCLUDE:WinMain")
  endif()

  if((CMAKE_CXX_COMPILER_ID STREQUAL GNU) AND (CMAKE_BUILD_TYPE MATCHES Debug) AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 10.0))
    message(STATUS "GUIPP: Debug build!")
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -O0 -D_FORTIFY_SOURCE=0 -fno-inline)
  else ()
    message(STATUS "GUIPP: Release build!")
  endif()

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    set(GUIPP_LINK STATIC)
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_BUILT_AS_STATIC)
    set(GUIPP_LIBRARIES ${GUIPP_LIBRARIES}
      guipp::ctrl
      guipp::look
      guipp::io
      guipp::draw
      guipp::layout
      guipp::win
      guipp::core
      ${LOGGING_LIBRARIES}
      ${UTIL_LIBRARIES}
      guipp::guipp
      )
    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES}
      ${LOGGING_SYS_LIBRARIES}
      ${UTIL_SYS_LIBRARIES})
  elseif(GUIPP_BUILD_SHARED_MODULE_LIBS)
    set(GUIPP_LINK SHARED)
    set(GUIPP_LIBRARIES ${GUIPP_LIBRARIES}
      guipp::ctrl
      guipp::look
      guipp::io
      guipp::draw
      guipp::layout
      guipp::win
      guipp::core
      ${LOGGING_LIBRARIES}
      ${UTIL_LIBRARIES}
      guipp::guipp
      )
    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES}
      ${LOGGING_SYS_LIBRARIES}
      ${UTIL_SYS_LIBRARIES})
  else()
    set(GUIPP_LINK OBJECT)
    set(GUIPP_LIBRARIES ${GUIPP_LIBRARIES}
      ${LOGGING_LIBRARIES}
      ${UTIL_LIBRARIES}
      guipp::guipp)
    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES}
      ${LOGGING_SYS_LIBRARIES}
      ${UTIL_SYS_LIBRARIES})
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS}
      -Dgui___EXPORTS
      -Dcore_EXPORTS
      -Dwin_EXPORTS
      -Dlayout_EXPORTS
      -Ddraw_EXPORTS
      -Dio_EXPORTS
      -Dctrl_EXPORTS
      -Dlook_EXPORTS
      )
  endif()

  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.5
     AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
     AND NOT CMAKE_CXX_COMPILER_ID STREQUAL MSVC
     AND NOT CMAKE_CXX_PLATFORM_ID STREQUAL MinGW)
    set(GUIPP_APP_LIBRARIES $<TARGET_OBJECTS:guipp::app> ${GUIPP_LIBRARIES})
  else()
    set(GUIPP_APP_LIBRARIES guipp::app ${GUIPP_LIBRARIES})
  endif()

  include_directories(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${GUIPP_INCLUDE_DIRS})

  add_subdirectory(core)
  add_subdirectory(win)
  add_subdirectory(layout)
  add_subdirectory(draw)
  add_subdirectory(io)
  add_subdirectory(ctrl)
  add_subdirectory(ctrl/look look)
  add_subdirectory(app)

  DebugPrint("gui GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
  DebugPrint("gui GUIPP_CXX_STANDARD: ${GUIPP_CXX_STANDARD}")
  DebugPrint("gui GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")
  DebugPrint("gui GUIPP_LIBRARIES: ${GUIPP_LIBRARIES}")
  DebugPrint("gui GUIPP_APP_LIBRARIES: ${GUIPP_APP_LIBRARIES}")
  DebugPrint("gui GUIPP_SYS_LIBRARIES: ${GUIPP_SYS_LIBRARIES}")
  DebugPrint("gui GUIPP_BUILD_FOR_MOBILE: ${GUIPP_BUILD_FOR_MOBILE}")
  DebugPrint("gui CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

  get_directory_property(HAS_PARENT PARENT_DIRECTORY)
  if(HAS_PARENT)
    DebugPrint("Set GUIPP_xyz to parent scope")
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} PARENT_SCOPE)
    set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
    set(GUIPP_LIBRARIES ${GUIPP_LIBRARIES} PARENT_SCOPE)
    set(GUIPP_APP_LIBRARIES ${GUIPP_APP_LIBRARIES} PARENT_SCOPE)
    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} PARENT_SCOPE)
    set(GUIPP_EXE_LINKER_FLAGS ${GUIPP_EXE_LINKER_FLAGS} PARENT_SCOPE)
    set(GUIPP_LINK_PROPERTIES ${GUIPP_LINK_PROPERTIES} PARENT_SCOPE)
    set(GUIPP_LINK_OPTIIONS ${GUIPP_LINK_OPTIIONS} PARENT_SCOPE)
    set(GUIPP_CXX_STANDARD ${GUIPP_CXX_STANDARD} PARENT_SCOPE)
  endif()

  # header file to pass version settings to the source code
  configure_file (
    "${PROJECT_SOURCE_DIR}/Version.h.in"
    "${PROJECT_BINARY_DIR}/gui_version.h"
    )

  add_definitions(${GUIPP_CXX_FLAGS})

  if(NOT GUIPP_BUILD_STATIC_MODULE_LIBS AND NOT GUIPP_BUILD_SHARED_MODULE_LIBS)
    DebugPrint("gui build global shared gui library")

    add_definitions(-Dgui___EXPORTS)

    add_library(guipp SHARED
      $<TARGET_OBJECTS:guipp::core>
      $<TARGET_OBJECTS:guipp::win>
      $<TARGET_OBJECTS:guipp::layout>
      $<TARGET_OBJECTS:guipp::draw>
      $<TARGET_OBJECTS:guipp::io>
      $<TARGET_OBJECTS:guipp::ctrl>
      $<TARGET_OBJECTS:guipp::look>
      gui.cpp
      )

    target_link_options(guipp PRIVATE "${GUIPP_EXE_LINKER_FLAGS}")
    target_link_libraries(guipp PRIVATE ${LINKED_LIBRARIES} ${GUIPP_SYS_LIBRARIES})

  elseif(GUIPP_BUILD_SHARED_MODULE_LIBS)
    DebugPrint("gui build multiple shared gui libraries")

    add_definitions(-Dgui___EXPORTS)

    add_library(guipp SHARED gui.cpp)

    target_link_options(guipp PRIVATE "${GUIPP_EXE_LINKER_FLAGS}")
    target_link_libraries(guipp PRIVATE ${GUIPP_SYS_LIBRARIES})

    if(UTIL_BUILD_STATIC_MODULE_LIB)
      target_link_libraries(guipp PRIVATE ${UTIL_LIBRARIES})
    endif()
    if(LOGGING_BUILD_STATIC_MODULE_LIB)
      target_link_libraries(guipp PRIVATE ${LOGGING_LIBRARIES})
    endif()

  else()
    DebugPrint("gui build multiple static gui libraries")

    add_library(guipp STATIC gui.cpp)

  endif()

  add_library(guipp::guipp ALIAS guipp)

  set_target_properties(guipp PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        LIBRARY_OUTPUT_NAME guipp${TARGET_SUFFIX}
                        VERSION ${GUIPP_VERSION}
                        SOVERSION ${GUIPP_VERSION_MAJOR}
                        CXX_STANDARD ${GUIPP_CXX_STANDARD})

  GENERATE_EXPORT_HEADER(guipp
                         BASE_NAME GUIPP
                         EXPORT_MACRO_NAME GUIPP_EXPORT
                         EXPORT_FILE_NAME gui++-export.h
                         STATIC_DEFINE GUIPP_BUILT_AS_STATIC)

  if(GUIPP_CONFIG_INSTALL)

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/guipp${TARGET_SUFFIX}ConfigVersion.cmake
        VERSION "${GUIPP_VERSION}"
        COMPATIBILITY SameMajorVersion)

    install_guipp_subpackage(guipp
        NAMESPACE guipp
        SUBDIR "."
        EXPORT_LIBS
        FILES ${INCLUDE_FILES}
              ${PROJECT_BINARY_DIR}/gui++-export.h
              ${PROJECT_BINARY_DIR}/gui_version.h
    )

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/guipp${TARGET_SUFFIX}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/guipp${TARGET_SUFFIX}
        PATH_VARS CMAKE_INSTALL_INCLUDEDIR
        NO_SET_AND_CHECK_MACRO
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/guipp${TARGET_SUFFIX}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/guipp${TARGET_SUFFIX}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/guipp${TARGET_SUFFIX}
    )

    install(
      EXPORT guipp${TARGET_SUFFIX}-targets
      NAMESPACE guipp::
      FILE guipp${TARGET_SUFFIX}Targets.cmake
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/guipp${TARGET_SUFFIX}
    )

  endif()

endif()
