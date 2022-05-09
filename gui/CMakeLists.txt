cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project("gui" LANGUAGES CXX VERSION 1.1.1)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (GenerateExportHeader)
include (CMakePackageConfigHelpers)
include (GNUInstallDirs)
include (CMakeParseArguments)

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
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

option(GUIPP_BUILD_STATIC_MODULE_LIBS "On to build a static library for each submodule, Off to build shared libraries. Default Off" OFF)
option(GUIPP_BUILD_SHARED_MODULE_LIBS "On to build a shared library for each submodule, Off to build static libraries. default Off" OFF)
option(GUIPP_CONFIG_INSTALL "On to configure installation, Off to build as part of a project. Default Off" OFF)
option(GUIPP_BUILD_FOR_MOBILE "On to optimize for smartphone, Off to optimize for dekstop. Default Off" OFF)
option(GUIPP_BUILD_DEPENDENT_LIBS "On to build dependent libs logging, util and testlib.  Default Off" OFF)
option(GUIPP_USE_XCB "On to make use of the xcb library. Default Off" OFF)
option(GUIPP_USE_QT5 "On to make use of the Qt5 library. Default Off" OFF)
option(GUIPP_USE_QT6 "On to make use of the Qt6 library. Default Off" OFF)
option(GUIPP_CXX_STANDARD "C++ standard to overwrite default cmake standard" ${CMAKE_CXX_STANDARD})
option(VERBOSE "Print messages during cmake run" OFF)

function(DebugPrint MSG)
  if(VERBOSE)
    message(STATUS "${MSG}")
  endif()
endfunction(DebugPrint)

if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")
  set(EMSCRIPTEN 1)
endif()

function(install_guipp_subpackage TARGET)

  if(GUIPP_CONFIG_INSTALL)
    cmake_parse_arguments(
        ARG           # prefix
        "EXPORT_LIBS" # boolean arguments
        "PREFIX;CONFIG;SUBDIR;NAMESPACE" # mono-valued arguments
        "FILES"       # multi-valued arguments
        ${ARGN}       # num of arguments of the function to parse
    )
    if((NOT TARGET) OR (NOT ARG_CONFIG) OR (NOT ARG_SUBDIR))
        message(FATAL_ERROR "You must provide target, subdir and config")
    endif()
    if((NOT ARG_PREFIX) AND (NOT ARG_NAMESPACE))
        message(FATAL_ERROR "You must provide either prefix or namespace")
    endif()
    if(NOT ARG_NAMESPACE)
      set(ARG_NAMESPACE ${ARG_PREFIX})
    endif()

    set(${ARG_PREFIX}${TARGET}_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/gui/${ARG_SUBDIR})
    set(${ARG_PREFIX}${TARGET}_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${ARG_NAMESPACE})

    target_include_directories(${TARGET}
                               PUBLIC
                               "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                               "$<INSTALL_INTERFACE:${${ARG_PREFIX}${TARGET}_INSTALL_INCLUDEDIR}>")

    configure_package_config_file(
        ${ARG_CONFIG}
        ${CMAKE_CURRENT_BINARY_DIR}/${ARG_PREFIX}${TARGET}Config.cmake
        INSTALL_DESTINATION ${${ARG_PREFIX}${TARGET}_INSTALL_CMAKEDIR}
        PATH_VARS CMAKE_INSTALL_INCLUDEDIR
    )

    if(ARG_EXPORT_LIBS OR GUIPP_BUILD_STATIC_MODULE_LIBS OR GUIPP_BUILD_SHARED_MODULE_LIBS)
      install(
          TARGETS ${TARGET}
          EXPORT ${ARG_PREFIX}${TARGET}_Targets
          OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
          INCLUDES DESTINATION ${${ARG_PREFIX}${TARGET}_INSTALL_INCLUDEDIR}
      )

      install(
          EXPORT ${ARG_PREFIX}${TARGET}_Targets
          NAMESPACE ${ARG_NAMESPACE}::
          FILE ${ARG_PREFIX}${TARGET}Targets.cmake
          DESTINATION ${${ARG_PREFIX}${TARGET}_INSTALL_CMAKEDIR}
      )

      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${ARG_PREFIX}${TARGET}Config.cmake
          DESTINATION ${${ARG_PREFIX}${TARGET}_INSTALL_CMAKEDIR}
      )

      export(TARGETS ${TARGET} FILE ${ARG_PREFIX}${TARGET}Targets.cmake)

    endif()

    if(EXISTS "${PROJECT_BINARY_DIR}/gui++-${TARGET}-export.h")
      list(APPEND ARG_FILES "${PROJECT_BINARY_DIR}/gui++-${TARGET}-export.h")
    endif()

    install(FILES ${ARG_FILES}
        DESTINATION ${${ARG_PREFIX}${TARGET}_INSTALL_INCLUDEDIR}
    )

  endif()

endfunction(install_guipp_subpackage)

function(define_guipp_subpackage TARGET)

  cmake_parse_arguments(
    ARG             # prefix
    "STATIC;SHARED" # boolean arguments
    "BASE;PREFIX;VERSION;VERSION_MAJOR;CXX_STANDARD;LINK" # mono-valued arguments
    "FLAGS;SOURCES;LIBS"    # multi-valued arguments
    ${ARGN}         # num of arguments to parse
  )
  if(NOT TARGET)
    message(FATAL_ERROR "You must provide TARGET")
  endif()
  if(NOT ARG_BASE)
    message(FATAL_ERROR "You must provide BASE")
  endif()
  if(NOT ARG_FLAGS)
    set(ARG_FLAGS ${GUIPP_CXX_FLAGS})
  endif()
  if(NOT ARG_PREFIX)
    set(ARG_PREFIX libguipp)
  endif()
  if(NOT ARG_LINK)
    set(ARG_LINK ${GUIPP_LINK})
  endif()
  if(NOT ARG_VERSION)
    set(ARG_VERSION ${GUIPP_VERSION})
  endif()
  if(NOT ARG_VERSION_MAJOR)
    set(ARG_VERSION_MAJOR ${GUIPP_VERSION_MAJOR})
  endif()
  if(NOT ARG_CXX_STANDARD)
    set(ARG_CXX_STANDARD ${GUIPP_CXX_STANDARD})
  endif()

  add_definitions(${ARG_FLAGS})

  if(ARG_STATIC OR (NOT ARG_SHARED AND GUIPP_BUILD_STATIC_MODULE_LIBS))
    message(STATUS "Build " ${TARGET} " as static library ")
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    add_library(${TARGET} STATIC ${ARG_SOURCES})
  elseif(ARG_SHARED OR (NOT ARG_STATIC AND GUIPP_BUILD_SHARED_MODULE_LIBS))
    message(STATUS "Build " ${TARGET} " as shared library ")
    add_definitions(-Dgui___${TARGET}_EXPORTS)
    add_library(${TARGET} SHARED ${ARG_SOURCES})
    target_link_libraries(${TARGET} PRIVATE ${ARG_LIBS})
  else()
    message(STATUS "Build " ${TARGET} " as " ${ARG_LINK} " library ")
    add_library(${TARGET} ${ARG_LINK} ${ARG_SOURCES})
  endif()

  add_library(guipp::${TARGET} ALIAS ${TARGET})
  set_target_properties(${TARGET} PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        PREFIX ${ARG_PREFIX}
                        VERSION ${ARG_VERSION}
                        SOVERSION ${ARG_VERSION_MAJOR}
                        CXX_STANDARD ${ARG_CXX_STANDARD})

  GENERATE_EXPORT_HEADER(${TARGET}
    BASE_NAME ${ARG_BASE}
    EXPORT_MACRO_NAME ${ARG_BASE}_EXPORT
    EXPORT_FILE_NAME gui++-${TARGET}-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC)

endfunction(define_guipp_subpackage)

function(guipp_app TARGET)

  cmake_parse_arguments(
    ARG                             # prefix
    ""                              # boolean arguments
    "PACKAGE_NAME;MAIN_SRC;BIN_DIR" # mono-valued arguments
    "INCLUDES;SOURCES;LIBS"                  # multi-valued arguments
    ${ARGN}                         # num of arguments to parse
  )

  if(NOT TARGET)
    message(FATAL_ERROR "You must provide TARGET")
  endif()
  if(NOT ARG_MAIN_SRC)
    set(ARG_MAIN_SRC ${TARGET}.cpp)
  endif()
  if(NOT ARG_PACKAGE_NAME)
    set(ARG_PACKAGE_NAME "org.organization.${TARGET}")
  endif()
  if(NOT ARG_BIN_DIR)
    set(ARG_BIN_DIR bin)
  endif()

  include_directories(${GUIPP_INCLUDE_DIRS} ${ARG_INCLUDES})
  add_definitions(${GUIPP_CXX_FLAGS})

  if (ANDROID)
    add_library(${TARGET} SHARED ${ARG_MAIN_SRC} ${ARG_SOURCES})
  elseif (EMSCRIPTEN)
    add_executable(${TARGET} ${ARG_MAIN_SRC} ${ARG_SOURCES})
  else ()
    add_executable(${TARGET} WIN32 ${ARG_MAIN_SRC} ${ARG_SOURCES})
  endif ()

  target_link_libraries(${TARGET} ${ARG_LIBS} ${GUIPP_APP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
  install(TARGETS ${TARGET} DESTINATION ${ARG_BIN_DIR})
  set_target_properties(${TARGET} PROPERTIES
                        FOLDER apps
                        CXX_STANDARD ${GUIPP_CXX_STANDARD}
                        )
  if (EMSCRIPTEN)
    set_target_properties(${TARGET} PROPERTIES
                          LINK_FLAGS "-s USE_PTHREADS=1")
  endif()
  if (ANDROID)
    add_qt_android_apk(${TARGET}_apk ${TARGET}
                         PACKAGE_NAME ${ARG_PACKAGE_NAME}
                         INSTALL)
  endif()

endfunction(guipp_app)

if(GUIPP_USE_QT6)
    message(STATUS "GUIPP: build with Qt6!")
    find_package(Qt6Core)
    find_package(Qt6Gui)
elseif(GUIPP_USE_QT5)
    message(STATUS "GUIPP: build with Qt5!")
    find_package(Qt5Gui REQUIRED)
endif()

if(GUIPP_BUILD_DEPENDENT_LIBS)
  add_subdirectory(${CMAKE_SOURCE_DIR}/libs/logging logging)
  add_subdirectory(${CMAKE_SOURCE_DIR}/libs/util util)
else ()
  find_package(logging REQUIRED)
  find_package(util REQUIRED)
endif()

if(NOT TARGET guipp)

  if(GUIPP_CXX_STANDARD STREQUAL "OFF")
    set(GUIPP_CXX_STANDARD 17)
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    set(UTIL_BUILD_STATIC_MODULE_LIB OFF CACHE BOOL "disable static build")
    set(LOGGING_BUILD_STATIC_MODULE_LIB OFF CACHE BOOL "disable static build")
  endif()

  if(GUIPP_CONFIG_INSTALL AND (GUIPP_BUILD_SHARED_MODULE_LIBS OR GUIPP_BUILD_STATIC_MODULE_LIBS))
    set(UTIL_CONFIG_INSTALL ON CACHE BOOL "enable install")
    set(LOGGING_CONFIG_INSTALL ON CACHE BOOL "enable install")
    set(TESTING_CONFIG_INSTALL ON CACHE BOOL "enable install")
  endif()

  set(LOGGING_CXX_STANDARD ${GUIPP_CXX_STANDARD})
  set(UTIL_CXX_STANDARD ${GUIPP_CXX_STANDARD})

  if(WIN32)
    set(GUIPP_CXX_FLAGS ${LOGGING_CXX_FLAGS} ${UTIL_CXX_FLAGS} -DWIN32_LEAN_AND_MEAN -D_USE_MATH_DEFINES)
  else ()
    set(GUIPP_CXX_FLAGS ${LOGGING_CXX_FLAGS} ${UTIL_CXX_FLAGS})
  endif()

  if(ANDROID)
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DANDROID)
    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} log)
  endif()

#  if(EMSCRIPTEN)
#    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -pthread -matomics)
#    set(GUIPP_EXE_LINKER_FLAGS ${GUIPP_EXE_LINKER_FLAGS} -Wl --shared-memory --no-check-features)
#  endif()

  if(GUIPP_USE_BOOST)
    find_package(Boost 1.65 REQUIRED COMPONENTS filesystem)
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DUSE_BOOST)
    if(NOT WIN32)
      set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} dl)
    endif()
  endif()

  if(GUIPP_BUILD_FOR_MOBILE)
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_BUILD_FOR_MOBILE)
  endif()

  set(GUIPP_INCLUDE_DIRS
    ${LOGGING_INCLUDE_DIRS}
    ${UTIL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_BINARY_DIR}
    ${PROJECT_BINARY_DIR}
    )

  if(GUIPP_USE_QT6)
    DebugPrint("GUIPP: build with Qt6!")

    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${Qt6Gui_LIBRARIES})
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_QT ${Qt6Gui_DEFINITIONS})
    if(APPLE)
      set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -fPIC)
    elseif(ANDROID)
    elseif(EMSCRIPTEN)
    elseif(UNIX)
      set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -fPIC)
      set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} stdc++fs)
    endif()
    set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS})

    DebugPrint("GUIPP: Qt6Gui_DEFINITIONS: " ${Qt6Gui_DEFINITIONS})
    DebugPrint("GUIPP: Qt6Gui_INCLUDE_DIRS: " ${Qt6Gui_INCLUDE_DIRS})
    DebugPrint("GUIPP: Qt6Gui_LIBRARIES: " ${Qt6Gui_LIBRARIES})

  elseif(GUIPP_USE_QT5)
    DebugPrint("GUIPP: build with Qt5!")
#    find_package(Qt5Gui REQUIRED)

    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${Qt5Gui_LIBRARIES})
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_QT ${Qt5Gui_DEFINITIONS})
    if(APPLE)
      set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -fPIC)
    elseif(ANDROID)
    elseif(EMSCRIPTEN)
    elseif(UNIX)
      set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -fPIC)
      set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} stdc++fs)
    endif()
    set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS})

    DebugPrint("GUIPP: Qt5Gui_DEFINITIONS: " ${Qt5Gui_DEFINITIONS})
    DebugPrint("GUIPP: Qt5Gui_INCLUDE_DIRS: " ${Qt5Gui_INCLUDE_DIRS})
    DebugPrint("GUIPP: Qt5Gui_LIBRARIES: " ${Qt5Gui_LIBRARIES})

  elseif(APPLE)
    DebugPrint("GUIPP: build for Apple!")
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DCOCOA)

  elseif(UNIX)
    DebugPrint("GUIPP: build for Unix!")
    find_package(X11 REQUIRED)
    if(X11_FOUND)
      DebugPrint("GUIPP: X11 found!")
      if(X11_Xdmcp_FOUND)
        DebugPrint("GUIPP: X11_Xdmcp found!")
      else()
        message(SEND_ERROR "Xdmcp library is required!")
      endif()
      set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${X11_LIBRARIES} ${X11_Xdmcp_LIB} stdc++fs)
      set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_X11)
      set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS})

      if(X11_Xrender_FOUND)
        DebugPrint("GUIPP: X11_Xrender found!")
      endif()

      find_package(Freetype)
      if(Freetype_FOUND)
        DebugPrint("GUIPP: Freetype found!")
      endif()

      if(X11_Xft_FOUND)
        DebugPrint("GUIPP: X11_Xft found!")
        if(NOT X11_Xrender_FOUND)
          message(SEND_ERROR "Xrender library is required!")
        endif()
        if(NOT Freetype_FOUND)
          message(SEND_ERROR "Freetype library is required!")
        endif()
        set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${X11_Xft_LIB} ${X11_Xrender_LIB} ${FREETYPE_LIBRARIES} fontconfig)
        set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_USE_XFT)
        set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${X11_Xft_INCLUDE_PATH} ${FREETYPE_INCLUDE_DIRS})
      else()
        DebugPrint("GUIPP: X11_Xft NOT found!")
      endif()

      if(X11_Xrandr_FOUND)
        DebugPrint("GUIPP: X11_Xrandr found!")
        set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_USE_XRANDR)
        set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${X11_Xrandr_LIB})
      endif()

      if(X11_X11_xcb_FOUND)
        message (STATUS "GUIPP: xcb found!")
        set(GUIPP_USE_XCB ON)
      endif()

    else()
      message(SEND_ERROR "X11 libraries are required!")
    endif(X11_FOUND)
    #        set(GUIPP_SYS_LIBRARIES X11 Xft fontconfig xcb Xdmcp bsd dl freetype Xrender expat Xau z png16)

  elseif(CMAKE_CXX_PLATFORM_ID STREQUAL MinGW)
    DebugPrint("GUIPP: build with MinGW for Win32!")
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_WIN -DUSE_MINGW -D_WIN32_WINNT=0x0601)
    set(GUIPP_EXE_LINKER_FLAGS ${GUIPP_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static)
    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} mingw32)
  elseif(WIN32)
    DebugPrint("GUIPP: build for Win32!")
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_WIN)
    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} Shcore)
  endif()

  if(CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
    set(GUIPP_EXE_LINKER_FLAGS ${GUIPP_EXE_LINKER_FLAGS} /Y- /bigobj)
  endif()

  if((CMAKE_CXX_COMPILER_ID STREQUAL GNU) AND (CMAKE_BUILD_TYPE MATCHES Debug) AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 10.0))
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -O0 -fno-inline -U_FORTIFY_SOURCE)
  else ()
    message(STATUS "GUIPP: Release build!")
  endif()

  if(GUIPP_USE_XCB)
    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} X11-xcb xcb xcb-xrm)
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_USE_XCB)
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

  set(LINKED_LIBRARIES ${LOGGING_LIBRARIES} ${UTIL_LIBRARIES})

  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.5 AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(GUIPP_APP_LIBRARIES $<TARGET_OBJECTS:guipp::app> ${GUIPP_LIBRARIES})
  else()
    set(GUIPP_APP_LIBRARIES guipp::app ${GUIPP_LIBRARIES})
  endif()

  include_directories(${GUIPP_INCLUDE_DIRS})

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
#  DebugPrint("gui GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")
  DebugPrint("gui GUIPP_LIBRARIES: ${GUIPP_LIBRARIES}")
  DebugPrint("gui GUIPP_APP_LIBRARIES: ${GUIPP_APP_LIBRARIES}")
  DebugPrint("gui GUIPP_SYS_LIBRARIES: ${GUIPP_SYS_LIBRARIES}")
  DebugPrint("gui GUIPP_BUILD_FOR_MOBILE: ${GUIPP_BUILD_FOR_MOBILE}")

  get_directory_property(HAS_PARENT PARENT_DIRECTORY)
  if(HAS_PARENT)
    DebugPrint("Set GUIPP_xyz to parent scope")
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} PARENT_SCOPE)
    set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
    set(GUIPP_LIBRARIES ${GUIPP_LIBRARIES} PARENT_SCOPE)
    set(GUIPP_APP_LIBRARIES ${GUIPP_APP_LIBRARIES} PARENT_SCOPE)
    set(GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} PARENT_SCOPE)
    set(GUIPP_EXE_LINKER_FLAGS ${GUIPP_EXE_LINKER_FLAGS} PARENT_SCOPE)
    set(GUIPP_CXX_STANDARD ${GUIPP_CXX_STANDARD} PARENT_SCOPE)
  endif()

#  include_directories(${GUIPP_INCLUDE_DIRS})

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

    target_link_options(guipp PRIVATE ${GUIPP_EXE_LINKER_FLAGS})
    target_link_libraries(guipp PRIVATE ${LOGGING_LIBRARIES} ${UTIL_LIBRARIES} ${GUIPP_SYS_LIBRARIES})

  elseif(GUIPP_BUILD_SHARED_MODULE_LIBS)
    DebugPrint("gui build multiple shared gui libraries")

    add_definitions(-Dgui___EXPORTS)

    add_library(guipp SHARED gui.cpp)

    target_link_options(guipp PRIVATE ${GUIPP_EXE_LINKER_FLAGS})
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
      guippConfigVersion.cmake
      VERSION "${GUIPP_VERSION}"
      COMPATIBILITY SameMajorVersion)
  endif()

  install_guipp_subpackage(guipp
    NAMESPACE guipp
    CONFIG ${PROJECT_SOURCE_DIR}/Config.cmake.in
    SUBDIR "."
    EXPORT_LIBS
    FILES ${INCLUDE_FILES}
          ${PROJECT_BINARY_DIR}/gui++-export.h
          ${PROJECT_BINARY_DIR}/gui_version.h
  )

endif()
