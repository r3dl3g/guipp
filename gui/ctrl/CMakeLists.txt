cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

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
      number_edit.cpp
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
      uneven_list.cpp
      tooltip.cpp
  )

  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  define_guipp_subpackage(${PROJECT_NAME}
                          BASE GUIPP_CTRL
                          SOURCES ${SOURCE_FILES} ${INCLUDE_FILES}
                          LIBS guipp::core guipp::win guipp::draw guipp::io guipp::layout guipp::look
                               ${LOGGING_LIBRARIES} ${UTIL_LIBRARIES} ${GUIPP_SYS_LIBRARIES})

  install_guipp_subpackage(${PROJECT_NAME}
                           PREFIX guipp
                           SUBDIR ${PROJECT_NAME}
                           CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/../SubConfig.cmake.in
                           FILES ${INCLUDE_FILES})

endif()
