cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("win" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../core core)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  set(SOURCE_FILES
      background_repeater.cpp
      clipboard.cpp
      container.cpp
      cursor.cpp
      dbg_win_message.cpp
      native_qt.cpp
      native_win32.cpp
      native_x11.cpp
      overlapped_window.cpp
      receiver.cpp
      window.cpp
      window_class.cpp
      window_event_handler.cpp
      window_event_proc.cpp
  )

  set(INCLUDE_FILES
      clipboard.h
      container.h
      container_class_qt.h
      container_class_win32.h
      container_class_x11.h
      cursor.h
      cursor.inl
      dbg_win_message.h
      native.h
      overlapped_window.h
      overlapped_window.inl
      receiver.h
      receiver.inl
      window.h
      window.inl
      window_class.h
      window_event_handler.h
      window_event_handler_qt.h
      window_event_handler_win32.h
      window_event_handler_x11.h
      window_event_proc.h
  )

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
                          ${LOGGING_LIBRARIES}
                          ${UTIL_LIBRARIES}
                          ${GUIPP_SYS_LIBRARIES})
  endif()

  GENERATE_EXPORT_HEADER(${PROJECT_NAME}
    BASE_NAME GUIPP_WIN
    EXPORT_MACRO_NAME GUIPP_WIN_EXPORT
    EXPORT_FILE_NAME gui++-${PROJECT_NAME}-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
  )

  if (GUIPP_STANDALONE_BUILD)
    write_subpackage_config(gui++ ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in "${INCLUDE_FILES}")
  endif()

endif()
