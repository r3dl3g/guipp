cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (GenerateExportHeader)

option(GUIPP_BUILD_STATIC_MODULE_LIBS "On to build a static library for each submodule, Off to build shared libraries. Default Off" OFF)
option(GUIPP_BUILD_SHARED_MODULE_LIBS "On to build a shared library for each submodule, Off to build static libraries. default Off" OFF)
option(GUIPP_USE_XCB "On to make use of the xcb library. Default Off" OFF)
option(GUIPP_STANDALONE_BUILD "On to make an installable standalone build, Off to build as part of a project. Default Off" OFF)
option(GUIPP_BUILD_FOR_MOBILE "On to optimize for smartphone, Off to optimize for dekstop. Default Off" OFF)
option(GUIPP_USE_QT "On to make use of the Qt library. Default Off" OFF)

if(NOT TARGET gui++ AND NOT TARGET gui++-static)

  add_subdirectory(../logging logging)
  add_subdirectory(../util util)

  set (GUIPP_VERSION_MAJOR 0)
  set (GUIPP_VERSION_MINOR 1)
  set (GUIPP_VERSION_PATCH 0)
  set (GUIPP_VERSION "${GUIPP_VERSION_MAJOR}.${GUIPP_VERSION_MINOR}.${GUIPP_VERSION_PATCH}")

  if (WIN32)
    set (GUIPP_CXX_FLAGS ${LOGGING_CXX_FLAGS} ${UTIL_CXX_FLAGS} -DWIN32_LEAN_AND_MEAN)
  else ()
    set (GUIPP_CXX_FLAGS ${LOGGING_CXX_FLAGS} ${UTIL_CXX_FLAGS})
  endif ()

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

  set (INCLUDE_INSTALL_DIR include/guipp/gui)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  if (GUIPP_USE_QT)
    message(STATUS "GUIPP: build with Qt5!")
    find_package(Qt5Widgets REQUIRED)

    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES})
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} ${Qt5Widgets_DEFINITIONS} -lstdc++fs -fPIC)
    set (GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${Qt5Widgets_INCLUDES} /usr/local/src/qt-everywhere-opensource-src-5.9.5/qtbase/include)

    message(STATUS "GUIPP: Qt5Widgets_DEFINITIONS: " ${Qt5Widgets_DEFINITIONS})
    message(STATUS "GUIPP: Qt5Widgets_INCLUDES: " ${Qt5Widgets_INCLUDES})

  elseif (APPLE)
    message(STATUS "GUIPP: build for Apple!")
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DCOCOA -lstdc++fs)

  elseif (UNIX)
    message(STATUS "GUIPP: build for Unix!")
    find_package(X11 REQUIRED)
    if (X11_FOUND)
      message(STATUS "GUIPP: X11 found!")
      if (X11_Xdmcp_FOUND)
        message(STATUS "GUIPP: X11_Xdmcp found!")
      else()
        message(SEND_ERROR "Xdmcp library is required!")
      endif()
      set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${X11_LIBRARIES} ${X11_Xdmcp_LIB})
      set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DX11 -lstdc++fs)
      set (GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS})

      if (X11_Xrender_FOUND)
        message(STATUS "GUIPP: X11_Xrender found!")
      endif()

      find_package(Freetype)
      if (Freetype_FOUND)
        message(STATUS "GUIPP: Freetype found!")
      endif()

      if (X11_Xft_FOUND)
        message(STATUS "GUIPP: X11_Xft found!")
        if (NOT X11_Xrender_FOUND)
          message(SEND_ERROR "Xrender library is required!")
        endif()
        if (NOT Freetype_FOUND)
          message(SEND_ERROR "Freetype library is required!")
        endif()
        set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${X11_Xft_LIB} ${X11_Xrender_LIB} ${FREETYPE_LIBRARIES} fontconfig)
        set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DUSE_XFT)
        set (GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${X11_Xft_INCLUDE_PATH} ${FREETYPE_INCLUDE_DIRS})
      else()
        message(STATUS "GUIPP: X11_Xft NOT found!")
      endif()

      if (X11_X11_xcb_FOUND)
        message (STATUS "GUIPP: xcb found!")
        set (GUIPP_USE_XCB ON)
      endif()

    else()
      message(SEND_ERROR "X11 libraries are required!")
    endif(X11_FOUND)
    #        set (GUIPP_SYS_LIBRARIES X11 Xft fontconfig xcb Xdmcp bsd dl freetype Xrender expat Xau z png16)

  elseif (WIN32)
    message(STATUS "GUIPP: build for Win32!")
    set (GUIPP_SYS_LIBRARIES Shcore)
  endif ()

  if (GUIPP_USE_XCB)
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} X11-xcb xcb xcb-xrm)
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DXCB)
  endif ()

  if (GUIPP_BUILD_STATIC_MODULE_LIBS)
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DGUIPP_BUILT_AS_STATIC)
    set (GUIPP_LIBRARIES ${GUIPP_LIBRARIES}
      gui++-ctrl-static
      gui++-io-static
      gui++-draw-static
      gui++-layout-static
      gui++-win-static
      gui++-core-static
      ${LOGGING_LIBRARIES}
      ${UTIL_LIBRARIES}
      gui++-static
      )
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${LOGGING_SYS_LIBRARIES} ${UTIL_SYS_LIBRARIES})
  elseif (GUIPP_BUILD_SHARED_MODULE_LIBS)
    set (GUIPP_LIBRARIES ${GUIPP_LIBRARIES}
      gui++-ctrl
      gui++-io
      gui++-draw
      gui++-layout
      gui++-win
      gui++-core
      ${LOGGING_LIBRARIES}
      ${UTIL_LIBRARIES}
      gui++
      )
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${LOGGING_SYS_LIBRARIES} ${UTIL_SYS_LIBRARIES})
  else()
    set (GUIPP_LIBRARIES ${GUIPP_LIBRARIES} ${LOGGING_LIBRARIES} ${UTIL_LIBRARIES} gui++)
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} ${LOGGING_SYS_LIBRARIES} ${UTIL_SYS_LIBRARIES})
  endif()

  set (GUIPP_APP_LIBRARIES
    gui++-app
    ${GUIPP_LIBRARIES}
    )

  if (NOT GUIPP_BUILD_STATIC_MODULE_LIBS AND NOT GUIPP_BUILD_SHARED_MODULE_LIBS)
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS}
      -Dgui___core_obj_EXPORTS
      -Dgui___win_obj_EXPORTS
      -Dgui___layout_obj_EXPORTS
      -Dgui___draw_obj_EXPORTS
      -Dgui___io_obj_EXPORTS
      -Dgui___ctrl_obj_EXPORTS)
  endif()

  add_subdirectory(core gui_core)
  add_subdirectory(win gui_win)
  add_subdirectory(layout gui_layout)
  add_subdirectory(draw gui_draw)
  add_subdirectory(io gui_io)
  add_subdirectory(ctrl gui_ctrl)
  add_subdirectory(app gui_app)

  get_directory_property(HAS_PARENT PARENT_DIRECTORY)
  if(HAS_PARENT)
    set (GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} PARENT_SCOPE)
    set (GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
    set (GUIPP_LIBRARIES ${GUIPP_LIBRARIES} PARENT_SCOPE)
    set (GUIPP_APP_LIBRARIES ${GUIPP_APP_LIBRARIES} PARENT_SCOPE)
    set (GUIPP_SYS_LIBRARIES ${GUIPP_SYS_LIBRARIES} PARENT_SCOPE)
  endif()

  message(STATUS "gui GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
  message(STATUS "gui GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")
  message(STATUS "gui GUIPP_LIBRARIES: ${GUIPP_LIBRARIES}")
  message(STATUS "gui GUIPP_APP_LIBRARIES: ${GUIPP_APP_LIBRARIES}")
  message(STATUS "gui GUIPP_SYS_LIBRARIES: ${GUIPP_SYS_LIBRARIES}")
  message(STATUS "gui GUIPP_BUILD_FOR_MOBILE: ${GUIPP_BUILD_FOR_MOBILE}")

  # header file to pass version settings to the source code
  configure_file (
    "${PROJECT_SOURCE_DIR}/Version.h.in"
    "${PROJECT_BINARY_DIR}/gui_version.h"
    )

  include_directories(${GUIPP_INCLUDE_DIRS})

  if (NOT GUIPP_BUILD_STATIC_MODULE_LIBS AND NOT GUIPP_BUILD_SHARED_MODULE_LIBS)
    message(STATUS "gui build global shared gui library")

    add_definitions(-Dgui___EXPORTS ${GUIPP_CXX_FLAGS})

    add_library(gui++ SHARED
      $<TARGET_OBJECTS:gui++-core-obj>
      $<TARGET_OBJECTS:gui++-win-obj>
      $<TARGET_OBJECTS:gui++-layout-obj>
      $<TARGET_OBJECTS:gui++-draw-obj>
      $<TARGET_OBJECTS:gui++-io-obj>
      $<TARGET_OBJECTS:gui++-ctrl-obj>
      gui.cpp
      )

    target_link_libraries(gui++ PRIVATE ${LOGGING_LIBRARIES} ${UTIL_LIBRARIES} ${GUIPP_SYS_LIBRARIES})

    set_target_properties(gui++ PROPERTIES
                          POSITION_INDEPENDENT_CODE ON
                          FOLDER libraries
                          VERSION ${GUIPP_VERSION}
                          SOVERSION ${GUIPP_VERSION_MAJOR}
                          CXX_STANDARD 11
    )

    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++ DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()

    GENERATE_EXPORT_HEADER(gui++
      BASE_NAME GUIPP
      EXPORT_MACRO_NAME GUIPP_EXPORT
      EXPORT_FILE_NAME gui++-export.h
      STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  elseif(GUIPP_BUILD_SHARED_MODULE_LIBS)
    message(STATUS "gui build multiple shared gui libraries")

    add_definitions(-Dgui___EXPORTS ${GUIPP_CXX_FLAGS})

    add_library(gui++ SHARED gui.cpp)

    target_link_libraries(gui++ PRIVATE ${GUIPP_SYS_LIBRARIES})

    set_target_properties(gui++ PROPERTIES
                          POSITION_INDEPENDENT_CODE ON
                          FOLDER libraries
                          VERSION ${GUIPP_VERSION}
                          SOVERSION ${GUIPP_VERSION_MAJOR}
                          CXX_STANDARD 11
    )

    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++ DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()

    GENERATE_EXPORT_HEADER(gui++
      BASE_NAME GUIPP
      EXPORT_MACRO_NAME GUIPP_EXPORT
      EXPORT_FILE_NAME gui++-export.h
      STATIC_DEFINE GUIPP_BUILT_AS_STATIC
      )

  else()
    message(STATUS "gui build multiple static gui libraries")

    add_definitions(${GUIPP_CXX_FLAGS})

    add_library(gui++-static STATIC gui.cpp)

    set_target_properties(gui++-static PROPERTIES
                          POSITION_INDEPENDENT_CODE ON
                          FOLDER libraries
                          VERSION ${GUIPP_VERSION}
                          SOVERSION ${GUIPP_VERSION_MAJOR}
                          CXX_STANDARD 11
    )

    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-static DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()

    GENERATE_EXPORT_HEADER(gui++-static
      BASE_NAME GUIPP
      EXPORT_MACRO_NAME GUIPP_EXPORT
      EXPORT_FILE_NAME gui++-export.h
      STATIC_DEFINE GUIPP_BUILT_AS_STATIC
      )

  endif()

  if (GUIPP_STANDALONE_BUILD)
    install(FILES ${INCLUDE_FILES}
      ${PROJECT_BINARY_DIR}/gui++-export.h
      ${PROJECT_BINARY_DIR}/gui_version.h
      DESTINATION ${INCLUDE_INSTALL_DIR})
  endif()

endif()
