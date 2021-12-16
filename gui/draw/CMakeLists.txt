cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("draw" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../core core)

  set(SOURCE_FILES
      arc_coords.cpp
      bitmap_info.cpp
      datamap.cpp
      converter.cpp
      frames.cpp
      drawers.cpp
      drawers_qt.cpp
      drawers_win32.cpp
      drawers_x11.cpp
      brush.cpp
      diagram.cpp
      bitmap.cpp
      font.cpp
      graphics.cpp
      image_data.cpp
      pen.cpp
      text_origin.cpp
      use.cpp
      icons.cpp
      clock.cpp
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
