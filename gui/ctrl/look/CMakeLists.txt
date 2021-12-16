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

  define_guipp_subpackage(${PROJECT_NAME}
                          BASE GUIPP_LOOK
                          PREFIX libguipp
                          LINK ${GUIPP_LINK}
                          VERSION ${GUIPP_VERSION}
                          VERSION_MAJOR ${GUIPP_VERSION_MAJOR}
                          CXX_STANDARD ${GUIPP_CXX_STANDARD}
                          SOURCES ${SOURCE_FILES} ${INCLUDE_FILES}
                          LIBS guipp::core guipp::draw guipp::io
                               ${UTIL_LIBRARIES} ${GUIPP_SYS_LIBRARIES})

  configure_guipp_subpackage_config(${PROJECT_NAME}
                                    PREFIX guipp
                                    SUBDIR ctrl/${PROJECT_NAME}
                                    CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/../../SubConfig.cmake.in
                                    INCLUDES ${INCLUDE_FILES})

  if (GUIPP_CONFIG_INSTALL)
    install(FILES ${RESOURCE_FILES}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gui/ctrl/${PROJECT_NAME}/res)
  endif()

endif()
