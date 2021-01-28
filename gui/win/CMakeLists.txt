cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-win" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-win)

  add_subdirectory(../core gui_core)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  set (INCLUDE_INSTALL_DIR include/guipp/gui/win)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  set(SOURCE_FILES
      clipboard.cpp
      container.cpp
      cursor.cpp
      dbg_win_message.cpp
      native_qt.cpp
      native_win32.cpp
      native_x11.cpp
      receiver.cpp
      window.cpp
      window_class.cpp
      window_event_handler.cpp
      window_event_proc.cpp
  )

  set(INCLUDE_FILES
      clipboard.h
      container.h
      container.inl
      container_class_qt.h
      container_class_win32.h
      container_class_x11.h
      cursor.h
      cursor.inl
      dbg_win_message.h
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

  add_library(gui++-win ${GUIPP_LINK} ${SOURCE_FILES} ${INCLUDE_FILES})

  set_target_properties(gui++-win PROPERTIES
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
    add_definitions(-Dgui___win_EXPORTS)
    target_link_libraries(gui++-win PRIVATE
                          gui++-core
                          ${LOGGING_LIBRARIES}
                          ${UTIL_LIBRARIES}
                          ${GUIPP_SYS_LIBRARIES})
  endif()

  if (GUIPP_STANDALONE_BUILD)
    install(TARGETS gui++-win DESTINATION ${LIBRARY_INSTALL_DIR})
  endif()

  GENERATE_EXPORT_HEADER(gui++-win
    BASE_NAME GUIPP_WIN
    EXPORT_MACRO_NAME GUIPP_WIN_EXPORT
    EXPORT_FILE_NAME gui++-win-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
  )

  if (GUIPP_STANDALONE_BUILD)
    install(FILES ${INCLUDE_FILES}
      ${PROJECT_BINARY_DIR}/gui++-win-export.h
      DESTINATION ${INCLUDE_INSTALL_DIR})
  endif()

endif()
