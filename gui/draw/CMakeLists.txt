cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project("draw" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../core core)

  set(SOURCE_FILES
      arc_coords.cpp
      bitmap_info.cpp
      bitmap.cpp
      bitmap_js.cpp
      bitmap_qt.cpp
      bitmap_sdl.cpp
      bitmap_win32.cpp
      bitmap_x11.cpp
      brush.cpp
      clock.cpp
      converter.cpp
      datamap.cpp
      diagram.cpp
      drawers.cpp
      drawers_qt.cpp
      drawers_win32.cpp
      drawers_x11.cpp
      drawers_js.cpp
      font_win32.cpp
      font_x11.cpp
      font_qt.cpp
      font_js.cpp
      frames.cpp
      graphics.cpp
      graphics_js.cpp
      graphics_qt.cpp
      graphics_win32.cpp
      graphics_x11.cpp
      icons.cpp
      pen.cpp
      shared_datamap.cpp
      text_origin.cpp
      use_js.cpp
      use_qt.cpp
      use_x11.cpp
  )
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  define_guipp_subpackage(${PROJECT_NAME}
                          BASE GUIPP_DRAW
                          SOURCES ${SOURCE_FILES} ${INCLUDE_FILES}
                          LIBS guipp::core ${UTIL_LIBRARIES} ${GUIPP_SYS_LIBRARIES})

  install_guipp_subpackage(${PROJECT_NAME}
                           PREFIX guipp
                           SUBDIR ${PROJECT_NAME}
                           CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/../SubConfig.cmake.in
                           FILES ${INCLUDE_FILES})

endif()
