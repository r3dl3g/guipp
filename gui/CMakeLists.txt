cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (GenerateExportHeader)

option(GUIPP_BUILD_STATIC_MODULE_LIBS "On to build a static library for each submodule, Off to build shared libraries. Default Off" OFF)
option(GUIPP_BUILD_SHARED_MODULE_LIBS "On to build a shared library for each submodule, Off to build static libraries. default Off" OFF)
option(GUIPP_USE_XCB "On to make use uf the xcb library. Default Off" OFF)

if(NOT TARGET gui++ AND NOT TARGET gui++-static)

  add_subdirectory(../logging logging)
  add_subdirectory(../util util)

  set (gui_VERSION_MAJOR 0)
  set (gui_VERSION_MINOR 1)
  set (gui_VERSION_PATCH 0)
  set (gui_VERSION "${gui_VERSION_MAJOR}.${gui_VERSION_MINOR}.${gui_VERSION_PATCH}")

  if (WIN32)
    set (gui_defines ${logging_defines} ${util_defines} -DWIN32_LEAN_AND_MEAN)
  else ()
    set (gui_defines ${logging_defines} ${util_defines})
  endif ()

  set (gui_include
    ${logging_include}
    ${util_include}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_BINARY_DIR}
    )

  set (INCLUDE_INSTALL_DIR include/guipp/gui)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  if (APPLE)
    set (gui_defines ${gui_defines})
  elseif (UNIX)
    find_package(X11 REQUIRED)
    if (X11_FOUND)
      if (NOT X11_Xdmcp_FOUND)
        message(SEND_ERROR "Xdmcp library is required!")
      endif()
      set (gui_sys_libraries ${gui_sys_libraries} ${X11_LIBRARIES} ${X11_Xdmcp_LIB})
      set (gui_defines ${gui_defines} -DX11)
      set (gui_include ${gui_include})
      if (X11_Xft_FOUND)
        if (NOT X11_Xrender_FOUND)
          message(SEND_ERROR "Xrender library is required!")
        endif()
        find_package(Freetype REQUIRED)
        set (gui_sys_libraries ${gui_sys_libraries} ${X11_Xft_LIB} ${X11_Xrender_LIB} ${FREETYPE_LIBRARIES} fontconfig)
        set (gui_defines ${gui_defines} -DUSE_XFT)
        set (gui_include ${gui_include} ${X11_Xft_INCLUDE_PATH} ${FREETYPE_INCLUDE_DIRS})
      endif()
    else()
      message(SEND_ERROR "X11 libraries are required!")
    endif(X11_FOUND)

    #        set (gui_sys_libraries X11 Xft fontconfig xcb Xdmcp bsd dl freetype Xrender expat Xau z png16)
  elseif (WIN32)
    set (gui_sys_libraries Shcore)
  endif ()

  if (GUIPP_USE_XCB)
    set (gui_sys_libraries ${gui_sys_libraries} X11-xcb xcb xcb-xrm)
    set (gui_defines ${gui_defines} -DXCB)
  endif ()

  if (GUIPP_BUILD_STATIC_MODULE_LIBS)
    set (gui_defines ${gui_defines} -DGUIPP_BUILT_AS_STATIC)
    set (gui_libraries
      gui++-ctrl-static
      gui++-io-static
      gui++-draw-static
      gui++-layout-static
      gui++-win-static
      gui++-core-static
      ${logging_libraries}
      ${util_libraries}
      gui++-static
      )
    set (gui_sys_libraries ${gui_sys_libraries} ${logging_sys_libraries} ${util_sys_libraries})
  elseif (GUIPP_BUILD_SHARED_MODULE_LIBS)
    set (gui_libraries
      gui++-ctrl
      gui++-io
      gui++-draw
      gui++-layout
      gui++-win
      gui++-core
      ${logging_libraries}
      ${util_libraries}
      )
    set (gui_sys_libraries gui++ ${gui_sys_libraries} ${logging_sys_libraries} ${util_sys_libraries})
  else()
    set (gui_libraries ${logging_libraries} ${util_libraries} gui++)
    set (gui_sys_libraries ${gui_sys_libraries} ${logging_sys_libraries} ${util_sys_libraries})
  endif()

  set (gui_app_libraries
    gui++-app
    ${gui_libraries}
    )

  if (NOT GUIPP_BUILD_STATIC_MODULE_LIBS AND NOT GUIPP_BUILD_SHARED_MODULE_LIBS)
    set (gui_defines ${gui_defines}
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
    set (gui_defines ${gui_defines} PARENT_SCOPE)
    set (gui_include ${gui_include} PARENT_SCOPE)
    set (gui_libraries ${gui_libraries} PARENT_SCOPE)
    set (gui_app_libraries ${gui_app_libraries} PARENT_SCOPE)
    set (gui_sys_libraries ${gui_sys_libraries} PARENT_SCOPE)
  endif()

  message(STATUS "gui gui_defines: ${gui_defines}")
  message(STATUS "gui gui_include: ${gui_include}")
  message(STATUS "gui gui_libraries: ${gui_libraries}")
  message(STATUS "gui gui_app_libraries: ${gui_app_libraries}")
  message(STATUS "gui gui_sys_libraries: ${gui_sys_libraries}")

  # header file to pass version settings to the source code
  configure_file (
    "${PROJECT_SOURCE_DIR}/Version.h.in"
    "${PROJECT_BINARY_DIR}/gui_version.h"
    )

  include_directories(${gui_include})

  if (NOT GUIPP_BUILD_STATIC_MODULE_LIBS AND NOT GUIPP_BUILD_SHARED_MODULE_LIBS)
    message(STATUS "gui build global shared gui library")

    add_definitions(-Dgui___EXPORTS ${gui_defines})

    add_library(gui++ SHARED
      $<TARGET_OBJECTS:gui++-core-obj>
      $<TARGET_OBJECTS:gui++-win-obj>
      $<TARGET_OBJECTS:gui++-layout-obj>
      $<TARGET_OBJECTS:gui++-draw-obj>
      $<TARGET_OBJECTS:gui++-io-obj>
      $<TARGET_OBJECTS:gui++-ctrl-obj>
      gui.cpp
      )

    target_link_libraries(gui++ PRIVATE ${logging_libraries} ${util_libraries} ${gui_sys_libraries})

    set_target_properties(gui++ PROPERTIES
                          POSITION_INDEPENDENT_CODE ON
                          FOLDER libraries
                          VERSION ${gui_VERSION}
                          SOVERSION ${gui_VERSION_MAJOR}
                          CXX_STANDARD 11
    )

    install(TARGETS gui++ DESTINATION ${LIBRARY_INSTALL_DIR})

    GENERATE_EXPORT_HEADER(gui++
      BASE_NAME GUIPP
      EXPORT_MACRO_NAME GUIPP_EXPORT
      EXPORT_FILE_NAME gui++-export.h
      STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  elseif(GUIPP_BUILD_SHARED_MODULE_LIBS)
    message(STATUS "gui build multiple shared gui libraries")

    add_definitions(-Dgui___EXPORTS ${gui_defines})

    add_library(gui++ SHARED gui.cpp)

    target_link_libraries(gui++ PRIVATE ${gui_sys_libraries})

    set_target_properties(gui++ PROPERTIES
                          POSITION_INDEPENDENT_CODE ON
                          FOLDER libraries
                          VERSION ${gui_VERSION}
                          SOVERSION ${gui_VERSION_MAJOR}
                          CXX_STANDARD 11
    )

    install(TARGETS gui++ DESTINATION ${LIBRARY_INSTALL_DIR})

    GENERATE_EXPORT_HEADER(gui++
      BASE_NAME GUIPP
      EXPORT_MACRO_NAME GUIPP_EXPORT
      EXPORT_FILE_NAME gui++-export.h
      STATIC_DEFINE GUIPP_BUILT_AS_STATIC
      )

  else()
    message(STATUS "gui build multiple static gui libraries")

    add_definitions(${gui_defines})

    add_library(gui++-static STATIC gui.cpp)

    set_target_properties(gui++-static PROPERTIES
                          POSITION_INDEPENDENT_CODE ON
                          FOLDER libraries
                          VERSION ${gui_VERSION}
                          SOVERSION ${gui_VERSION_MAJOR}
                          CXX_STANDARD 11
    )

    install(TARGETS gui++-static DESTINATION ${LIBRARY_INSTALL_DIR})

    GENERATE_EXPORT_HEADER(gui++-static
      BASE_NAME GUIPP
      EXPORT_MACRO_NAME GUIPP_EXPORT
      EXPORT_FILE_NAME gui++-export.h
      STATIC_DEFINE GUIPP_BUILT_AS_STATIC
      )

  endif()

  install(FILES ${INCLUDE_FILES}
    ${PROJECT_BINARY_DIR}/gui++-export.h
    ${PROJECT_BINARY_DIR}/gui_version.h
    DESTINATION ${INCLUDE_INSTALL_DIR})

endif()
