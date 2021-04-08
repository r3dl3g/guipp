cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("win" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../core core)

  set(SOURCE_FILES
      animation.cpp
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
  file(GLOB INCLUDE_FILES "*.h" "*.inl")
  set(LINKED_LIBRARIES
      guipp::core
      ${LOGGING_LIBRARIES}
      ${UTIL_LIBRARIES})

  set(LINKED_SYS_LIBRARIES
      ${GUIPP_SYS_LIBRARIES})

  define_guipp_subpackage(${PROJECT_NAME} GUIPP_WIN "${SOURCE_FILES}" "${INCLUDE_FILES}" "${LINKED_LIBRARIES}" "${LINKED_SYS_LIBRARIES}")

  if (GUIPP_STANDALONE_BUILD)
    configure_guipp_subpackage_config(guipp ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../SubConfig.cmake.in "${INCLUDE_FILES}" "${LINKED_LIBRARIES}")
  endif()

endif()
