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

  add_definitions(${GUIPP_CXX_FLAGS})

  DebugPrint("ctrl GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
#  message(STATUS "ctrl GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")

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

  add_library(${PROJECT_NAME} ${GUIPP_LINK} ${SOURCE_FILES} ${INCLUDE_FILES})
  add_library(gui++::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
  set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "libgui++")

  set_target_properties(${PROJECT_NAME} PROPERTIES
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
    add_definitions(-Dgui___${PROJECT_NAME}_EXPORTS)
    target_link_libraries(${PROJECT_NAME} PRIVATE
                          gui++::core
                          gui++::win
                          gui++::draw
                          gui++::io
                          gui++::layout
                          gui++::look
                          ${LOGGING_LIBRARIES}
                          ${UTIL_LIBRARIES}
                          ${GUIPP_SYS_LIBRARIES})
  endif()

  GENERATE_EXPORT_HEADER(${PROJECT_NAME}
    BASE_NAME GUIPP_CTRL
    EXPORT_MACRO_NAME GUIPP_CTRL_EXPORT
    EXPORT_FILE_NAME gui++-${PROJECT_NAME}-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  if (GUIPP_STANDALONE_BUILD)
    write_subpackage_config(gui++ ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in "${INCLUDE_FILES}")
  endif()

endif()
