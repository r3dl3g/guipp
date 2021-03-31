cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("look" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../../draw draw)
  add_subdirectory(../../io io)

  set(SOURCE_FILES
      button.cpp
      control.cpp
      drop_down.cpp
      edit.cpp
      label.cpp
      menu.cpp
      progress_bar.cpp
      scroll_bar.cpp
      splitter.cpp
      table.cpp
      textbox.cpp
      tree.cpp
  )
  file(GLOB INCLUDE_FILES "*.h" "*.inl")
  file(GLOB RESOURCE_FILES "res/*.h")

  set(LINKED_LIBRARIES
      guipp::core
      guipp::draw
      guipp::io
      ${UTIL_LIBRARIES})

    set(LINKED_SYS_LIBRARIES
      ${GUIPP_SYS_LIBRARIES})

  define_guipp_subpackage(${PROJECT_NAME} GUIPP_LOOK "${SOURCE_FILES}" "${INCLUDE_FILES}" "${LINKED_LIBRARIES}" "${LINKED_SYS_LIBRARIES}")

  if (GUIPP_STANDALONE_BUILD)
    configure_guipp_subpackage_config(guipp ${PROJECT_NAME} ctrl/${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../SubConfig.cmake.in "${INCLUDE_FILES}" "${LINKED_LIBRARIES}")

    install(FILES ${RESOURCE_FILES}
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gui/ctrl/${PROJECT_NAME}/res)

  endif()

endif()
