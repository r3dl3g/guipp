cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (GenerateExportHeader)

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

option(GUIPP_BUILD_STATIC_MODULE_LIBS "On to build a static library for each submodule, Off to build shared libraries. Default Off" OFF)
option(GUIPP_BUILD_SHARED_MODULE_LIBS "On to build a shared library for each submodule, Off to build static libraries. default Off" OFF)
option(GUIPP_USE_XCB "On to make use of the xcb library. Default Off" OFF)
option(GUIPP_STANDALONE_BUILD "On to make an installable standalone build, Off to build as part of a project. Default Off" OFF)
option(GUIPP_BUILD_FOR_MOBILE "On to optimize for smartphone, Off to optimize for dekstop. Default Off" OFF)
option(GUIPP_USE_QT "On to make use of the Qt library. Default Off" OFF)
option(GUIPP_CXX_STANDARD "C++ standard to overwrite default cmake standard" ${CMAKE_CXX_STANDARD})
option(VERBOSE "Print messages during cmake run" OFF)

function(DebugPrint MSG)
  if(VERBOSE)
    message(STATUS "${MSG}")
  endif()
endfunction(DebugPrint)

if(NOT TARGET gui++)

  set (GUIPP_VERSION_MAJOR 0)
  set (GUIPP_VERSION_MINOR 1)
  set (GUIPP_VERSION_PATCH 0)
  set (GUIPP_VERSION "${GUIPP_VERSION_MAJOR}.${GUIPP_VERSION_MINOR}.${GUIPP_VERSION_PATCH}")

  if (GUIPP_CXX_STANDARD STREQUAL "OFF")
    set (GUIPP_CXX_STANDARD 17)
  endif ()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    set (UTIL_BUILD_STATIC_MODULE_LIB OFF)
    set (LOGGING_BUILD_STATIC_MODULE_LIB OFF)
  endif ()
  set (LOGGING_CXX_STANDARD ${GUIPP_CXX_STANDARD})
  set (UTIL_CXX_STANDARD ${GUIPP_CXX_STANDARD})
  add_subdirectory(../logging logging)
  add_subdirectory(../util util)

  if (GUIPP_USE_BOOST)
    find_package(Boost 1.65 REQUIRED COMPONENTS filesystem)
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DUSE_BOOST)
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} dl)
  endif()

  if (GUIPP_BUILD_FOR_MOBILE)
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_BUILD_FOR_MOBILE)
  endif ()

  set (GUIPP_INCLUDE_DIRS
    ${LOGGING_INCLUDE_DIRS}
    ${UTIL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_BINARY_DIR}
    )

  if (WIN32)
    set (GUIPP_CXX_FLAGS ${LOGGING_CXX_FLAGS} ${UTIL_CXX_FLAGS} -DWIN32_LEAN_AND_MEAN -D_USE_MATH_DEFINES)
  else ()
    set (GUIPP_CXX_FLAGS ${LOGGING_CXX_FLAGS} ${UTIL_CXX_FLAGS})
  endif ()

  if (GUIPP_USE_QT)
    DebugPrint("GUIPP: build with Qt5!")
    find_package(Qt5Gui REQUIRED)

    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${Qt5Gui_LIBRARIES})
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_QT ${Qt5Gui_DEFINITIONS})
    if (APPLE)
      set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -fPIC)
    elseif (UNIX)
      set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -lstdc++fs -fPIC)
    endif()
    set (GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS})
    # /usr/local/src/qt-everywhere-opensource-src-5.9.5/qtbase/include

    DebugPrint("GUIPP: Qt5Gui_DEFINITIONS: " ${Qt5Gui_DEFINITIONS})
    DebugPrint("GUIPP: Qt5Gui_INCLUDE_DIRS: " ${Qt5Gui_INCLUDE_DIRS})
    DebugPrint("GUIPP: Qt5Gui_LIBRARIES: " ${Qt5Gui_LIBRARIES})

  elseif (APPLE)
    DebugPrint("GUIPP: build for Apple!")
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DCOCOA)

  elseif (UNIX)
    DebugPrint("GUIPP: build for Unix!")
    find_package(X11 REQUIRED)
    if (X11_FOUND)
      DebugPrint("GUIPP: X11 found!")
      if (X11_Xdmcp_FOUND)
        DebugPrint("GUIPP: X11_Xdmcp found!")
      else()
        message(SEND_ERROR "Xdmcp library is required!")
      endif()
      set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${X11_LIBRARIES} ${X11_Xdmcp_LIB})
      set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_X11 -lstdc++fs)
      set (GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS})

      if (X11_Xrender_FOUND)
        DebugPrint("GUIPP: X11_Xrender found!")
      endif()

      find_package(Freetype)
      if (Freetype_FOUND)
        DebugPrint("GUIPP: Freetype found!")
      endif()

      if (X11_Xft_FOUND)
        DebugPrint("GUIPP: X11_Xft found!")
        if (NOT X11_Xrender_FOUND)
          message(SEND_ERROR "Xrender library is required!")
        endif()
        if (NOT Freetype_FOUND)
          message(SEND_ERROR "Freetype library is required!")
        endif()
        set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${X11_Xft_LIB} ${X11_Xrender_LIB} ${FREETYPE_LIBRARIES} fontconfig)
        set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_USE_XFT)
        set (GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${X11_Xft_INCLUDE_PATH} ${FREETYPE_INCLUDE_DIRS})
      else()
        DebugPrint("GUIPP: X11_Xft NOT found!")
      endif()

      if (X11_Xrandr_FOUND)
        DebugPrint("GUIPP: X11_Xrandr found!")
        set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_USE_XRANDR)
        set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${X11_Xrandr_LIB})
      endif()

      if (X11_X11_xcb_FOUND)
        message (STATUS "GUIPP: xcb found!")
        set (GUIPP_USE_XCB ON)
      endif()

    else()
      message(SEND_ERROR "X11 libraries are required!")
    endif(X11_FOUND)
    #        set (GUIPP_SYS_LIBRARIES X11 Xft fontconfig xcb Xdmcp bsd dl freetype Xrender expat Xau z png16)

  elseif (CMAKE_CXX_PLATFORM_ID STREQUAL "MinGW")
    DebugPrint("GUIPP: build with MinGW for Win32!")
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_WIN -DUSE_MINGW -D_WIN32_WINNT=0x0601)
    set (GUIPP_EXE_LINKER_FLAGS ${GUIPP_EXE_LINKER_FLAGS} "-static-libgcc -static-libstdc++ -static")
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} mingw32)
  elseif (WIN32)
    DebugPrint("GUIPP: build for Win32!")
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_WIN)
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} Shcore)
  endif ()

  if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    set (GUIPP_EXE_LINKER_FLAGS ${GUIPP_EXE_LINKER_FLAGS} /Y- /bigobj)
  endif()

  if (GUIPP_USE_XCB)
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} X11-xcb xcb xcb-xrm)
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_USE_XCB)
  endif ()

  if (GUIPP_BUILD_STATIC_MODULE_LIBS)
    set (GUIPP_LINK STATIC)
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_BUILT_AS_STATIC)
    set (GUIPP_LIBRARIES ${GUIPP_LIBRARIES}
      gui++-ctrl
      gui++-look
      gui++-io
      gui++-draw
      gui++-layout
      gui++-win
      gui++-core
      ${LOGGING_LIBRARIES}
      ${UTIL_LIBRARIES}
      gui++
      )
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES}
      ${LOGGING_SYS_LIBRARIES}
      ${UTIL_SYS_LIBRARIES})
  elseif (GUIPP_BUILD_SHARED_MODULE_LIBS)
    set (GUIPP_LINK SHARED)
    set (GUIPP_LIBRARIES ${GUIPP_LIBRARIES}
      gui++-ctrl
      gui++-look
      gui++-io
      gui++-draw
      gui++-layout
      gui++-win
      gui++-core
      ${LOGGING_LIBRARIES}
      ${UTIL_LIBRARIES}
      gui++
      )
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES}
      ${LOGGING_SYS_LIBRARIES}
      ${UTIL_SYS_LIBRARIES})
  else()
    set (GUIPP_LINK OBJECT)
    set (GUIPP_LIBRARIES ${GUIPP_LIBRARIES}
      ${LOGGING_LIBRARIES}
      ${UTIL_LIBRARIES}
      gui++)
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES}
      ${LOGGING_SYS_LIBRARIES}
      ${UTIL_SYS_LIBRARIES})
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS}
      -Dgui___core_EXPORTS
      -Dgui___win_EXPORTS
      -Dgui___layout_EXPORTS
      -Dgui___draw_EXPORTS
      -Dgui___io_EXPORTS
      -Dgui___ctrl_EXPORTS
      -Dgui___look_EXPORTS
      )
  endif()

  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.5)
    set (GUIPP_APP_LIBRARIES $<TARGET_OBJECTS:gui++-app> ${GUIPP_LIBRARIES})
  else()
    set (GUIPP_APP_LIBRARIES gui++-app ${GUIPP_LIBRARIES})
  endif()

  add_subdirectory(core gui_core)
  add_subdirectory(win gui_win)
  add_subdirectory(layout gui_layout)
  add_subdirectory(draw gui_draw)
  add_subdirectory(io gui_io)
  add_subdirectory(ctrl gui_ctrl)
  add_subdirectory(ctrl/look gui_look)
  add_subdirectory(app gui_app)

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
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} PARENT_SCOPE)
    set (GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
    set (GUIPP_LIBRARIES ${GUIPP_LIBRARIES} PARENT_SCOPE)
    set (GUIPP_APP_LIBRARIES ${GUIPP_APP_LIBRARIES} PARENT_SCOPE)
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} PARENT_SCOPE)
    set (GUIPP_EXE_LINKER_FLAGS ${GUIPP_EXE_LINKER_FLAGS} PARENT_SCOPE)
    set (GUIPP_CXX_STANDARD ${GUIPP_CXX_STANDARD} PARENT_SCOPE)
  endif()

  include_directories(${GUIPP_INCLUDE_DIRS})

  # header file to pass version settings to the source code
  configure_file (
    "${PROJECT_SOURCE_DIR}/Version.h.in"
    "${PROJECT_BINARY_DIR}/gui_version.h"
    )

  add_definitions(${GUIPP_CXX_FLAGS})

  if (NOT GUIPP_BUILD_STATIC_MODULE_LIBS AND NOT GUIPP_BUILD_SHARED_MODULE_LIBS)
    DebugPrint("gui build global shared gui library")

    add_definitions(-Dgui___EXPORTS)

    add_library(gui++ SHARED
      $<TARGET_OBJECTS:gui++-core>
      $<TARGET_OBJECTS:gui++-win>
      $<TARGET_OBJECTS:gui++-layout>
      $<TARGET_OBJECTS:gui++-draw>
      $<TARGET_OBJECTS:gui++-io>
      $<TARGET_OBJECTS:gui++-ctrl>
      $<TARGET_OBJECTS:gui++-look>
      gui.cpp
      )

    target_link_libraries(gui++ PRIVATE ${LOGGING_LIBRARIES} ${UTIL_LIBRARIES} ${GUIPP_SYS_LIBRARIES})

  elseif(GUIPP_BUILD_SHARED_MODULE_LIBS)
    DebugPrint("gui build multiple shared gui libraries")

    add_definitions(-Dgui___EXPORTS)

    add_library(gui++ SHARED gui.cpp)

    target_link_libraries(gui++ PRIVATE ${GUIPP_SYS_LIBRARIES})

    if (UTIL_BUILD_STATIC_MODULE_LIB)
      target_link_libraries(gui++ PRIVATE ${UTIL_LIBRARIES})
    endif()
    if (LOGGING_BUILD_STATIC_MODULE_LIB)
      target_link_libraries(gui++ PRIVATE ${LOGGING_LIBRARIES})
    endif()

  else()
    DebugPrint("gui build multiple static gui libraries")

    add_library(gui++ STATIC gui.cpp)

  endif()

  set_target_properties(gui++ PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        VERSION ${GUIPP_VERSION}
                        SOVERSION ${GUIPP_VERSION_MAJOR}
                        CXX_STANDARD ${GUIPP_CXX_STANDARD}
                        )

  GENERATE_EXPORT_HEADER(gui++
                         BASE_NAME GUIPP
                         EXPORT_MACRO_NAME GUIPP_EXPORT
                         EXPORT_FILE_NAME gui++-export.h
                         STATIC_DEFINE GUIPP_BUILT_AS_STATIC
                         )

  set (INCLUDE_INSTALL_DIR include/guipp/gui)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  if (GUIPP_STANDALONE_BUILD)
    install(TARGETS gui++ DESTINATION ${LIBRARY_INSTALL_DIR})
    install(FILES ${INCLUDE_FILES}
      ${PROJECT_BINARY_DIR}/gui++-export.h
      ${PROJECT_BINARY_DIR}/gui_version.h
      DESTINATION ${INCLUDE_INSTALL_DIR})
  endif()

endif()
