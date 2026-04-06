cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project("win" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../core core)

  set(SOURCE_FILES
      animation.cpp
      background_repeater.cpp
      clipboard_js.cpp
      clipboard_qt.cpp
      clipboard_sdl.cpp
      clipboard_win32.cpp
      clipboard_x11.cpp
      container.cpp
      cursor.cpp
      dbg_win_message.cpp
      enable_drag.cpp
      native_js.cpp
      native_qt.cpp
      native_sdl.cpp
      native_win32.cpp
      native_x11.cpp
      overlapped_window.cpp
      receiver.cpp
      stacked_view_controller.cpp
      window.cpp
      window_class.cpp
      window_event_handler_js.cpp
      window_event_handler_qt.cpp
      window_event_handler_sdl.cpp
      window_event_handler_win32.cpp
      window_event_handler_x11.cpp
      window_event_proc_js.cpp
      window_event_proc_qt.cpp
      window_event_proc_sdl.cpp
      window_event_proc_win32.cpp
      window_event_proc_x11.cpp
      window_event_proc.cpp
  )

  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  define_guipp_subpackage(${PROJECT_NAME}
                          BASE GUIPP_WIN
                          SOURCES ${SOURCE_FILES} ${INCLUDE_FILES}
                          LIBS guipp::core ${LOGGING_LIBRARIES} ${UTIL_LIBRARIES} ${GUIPP_SYS_LIBRARIES})

  install_guipp_subpackage(${PROJECT_NAME}
                           PREFIX guipp
                           SUBDIR ${PROJECT_NAME}
                           CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/../SubConfig.cmake.in
                           FILES ${INCLUDE_FILES})

endif()
