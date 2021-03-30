cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("ctrl" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../win win)
  add_subdirectory(../io io)
  add_subdirectory(look)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  set(SOURCE_FILES
      button.cpp
      column_list.cpp
      control.cpp
      drop_down.cpp
      edit.cpp
      editbox.cpp
      file_tree.cpp
      image.cpp
      label.cpp
      list.cpp
      menu.cpp
      progress_bar.cpp
      scroll_bar.cpp
      scroll_view.cpp
      separator.cpp
      splitter.cpp
      split_view.cpp
      std_dialogs.cpp
      table.cpp
      textbox.cpp
      tile_view.cpp
  )

  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  set(LINKED_LIBRARIES
    gui++::core
    gui++::win
    gui++::draw
    gui++::io
    gui++::layout
    gui++::look
    ${LOGGING_LIBRARIES}
    ${UTIL_LIBRARIES}
    ${GUIPP_SYS_LIBRARIES})

  define_guipp_subpackage(${PROJECT_NAME} GUIPP_CTRL "${SOURCE_FILES}" "${INCLUDE_FILES}" "${LINKED_LIBRARIES}")

  if (GUIPP_STANDALONE_BUILD)
    configure_guipp_subpackage_config(gui++ ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in "${INCLUDE_FILES}")
  endif()

endif()
