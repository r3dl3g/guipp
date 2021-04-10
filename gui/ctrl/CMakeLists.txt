cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("ctrl" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../win win)
  add_subdirectory(../io io)
  add_subdirectory(look)

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
      title_view.cpp
  )

  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  set(LINKED_LIBRARIES
    guipp::core
    guipp::win
    guipp::draw
    guipp::io
    guipp::layout
    guipp::look
    ${LOGGING_LIBRARIES}
    ${UTIL_LIBRARIES})

  set(LINKED_SYS_LIBRARIES
    ${GUIPP_SYS_LIBRARIES})

  define_guipp_subpackage(${PROJECT_NAME} GUIPP_CTRL "${SOURCE_FILES}" "${INCLUDE_FILES}" "${LINKED_LIBRARIES}" "${LINKED_SYS_LIBRARIES}")

  if (GUIPP_STANDALONE_BUILD)
    configure_guipp_subpackage_config(guipp ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../SubConfig.cmake.in "${INCLUDE_FILES}" "${LINKED_LIBRARIES}")
  endif()

endif()
