cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project("core" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

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
      selection_adjustment.cpp
  )
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  define_guipp_subpackage(${PROJECT_NAME}
                          BASE GUIPP_CORE
                          SOURCES ${SOURCE_FILES} ${INCLUDE_FILES}
                          LIBS ${LOGGING_LIBRARIES} ${GUIPP_SYS_LIBRARIES} ${LOGGING_SYS_LIBRARIES})

  install_guipp_subpackage(${PROJECT_NAME}
                           PREFIX guipp
                           SUBDIR ${PROJECT_NAME}
                           CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/../SubConfig.cmake.in
                           FILES ${INCLUDE_FILES})

endif()
