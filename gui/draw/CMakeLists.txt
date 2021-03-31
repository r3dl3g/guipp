cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("draw" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../core core)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

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
  set(LINKED_LIBRARIES
      guipp::core
      ${UTIL_LIBRARIES}
      ${GUIPP_SYS_LIBRARIES})

  define_guipp_subpackage(${PROJECT_NAME} GUIPP_DRAW "${SOURCE_FILES}" "${INCLUDE_FILES}" "${LINKED_LIBRARIES}")

  if (GUIPP_STANDALONE_BUILD)
    configure_guipp_subpackage_config(guipp ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in "${INCLUDE_FILES}")
  endif()

endif()
