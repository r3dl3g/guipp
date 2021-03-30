cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("core" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  set(SOURCE_FILES
      angle.cpp
      bits.cpp
      button_state.cpp
      color.cpp
      context.cpp
      event.cpp
      event_container.cpp
      guidefs.cpp
      keys.cpp
      list_state.cpp
      native_qt.cpp
      native_win32.cpp
      native_x11.cpp
      window_state.cpp
  )
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  set(LINKED_LIBRARIES
      ${LOGGING_LIBRARIES}
      ${GUIPP_SYS_LIBRARIES}
      ${LOGGING_SYS_LIBRARIES})

  define_guipp_subpackage(${PROJECT_NAME} GUIPP_CORE "${SOURCE_FILES}" "${INCLUDE_FILES}" "${LINKED_LIBRARIES}")

  if (GUIPP_STANDALONE_BUILD)
    configure_guipp_subpackage_config(gui++ ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in "${INCLUDE_FILES}")
  endif()

endif()
