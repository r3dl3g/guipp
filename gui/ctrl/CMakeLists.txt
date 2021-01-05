cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-ctrl" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-ctrl)

  add_subdirectory(../win gui_win)
  add_subdirectory(../io gui_io)
  add_subdirectory(look gui_look)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  DebugPrint("ctrl GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
#  message(STATUS "ctrl GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")

  set (INCLUDE_INSTALL_DIR include/guipp/gui/ctrl)
  set (LIBRARY_INSTALL_DIR lib/guipp)

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
      slider.cpp
      split_view.cpp
      std_dialogs.cpp
      table.cpp
      textbox.cpp
      tile_view.cpp
      tree.cpp
  )

  file(GLOB INCLUDE_FILES "*.h" "*.inl")
  file(GLOB RESOURCE_FILES "res/*.h")

  add_library(gui++-ctrl ${GUIPP_LINK} ${SOURCE_FILES} ${INCLUDE_FILES} ${RESOURCE_FILES})

  set_target_properties(gui++-ctrl PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        VERSION ${GUIPP_VERSION}
                        SOVERSION ${GUIPP_VERSION_MAJOR}
                        CXX_STANDARD ${GUIPP_CXX_STANDARD}
                        )

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___ctrl_EXPORTS)
    target_link_libraries(gui++-ctrl PRIVATE
                          gui++-core
                          gui++-win
                          gui++-draw
                          gui++-io
                          gui++-layout
                          gui++-look
                          ${LOGGING_LIBRARIES}
                          ${UTIL_LIBRARIES}
                          ${GUIPP_SYS_LIBRARIES})
  endif()

  if (GUIPP_STANDALONE_BUILD)
    install(TARGETS gui++-ctrl DESTINATION ${LIBRARY_INSTALL_DIR})
  endif()

  GENERATE_EXPORT_HEADER(gui++-ctrl
    BASE_NAME GUIPP_CTRL
    EXPORT_MACRO_NAME GUIPP_CTRL_EXPORT
    EXPORT_FILE_NAME gui++-ctrl-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  if (GUIPP_STANDALONE_BUILD)
    install(FILES ${INCLUDE_FILES}
      ${PROJECT_BINARY_DIR}/gui++-ctrl-export.h
      DESTINATION ${INCLUDE_INSTALL_DIR})

    install(FILES ${RESOURCE_FILES}
      DESTINATION ${INCLUDE_INSTALL_DIR}/res)
  endif()

endif()
