cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("look" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../../draw draw)
  add_subdirectory(../../io io)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  DebugPrint("look GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")

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

  add_library(${PROJECT_NAME} ${GUIPP_LINK} ${SOURCE_FILES} ${INCLUDE_FILES} ${RESOURCE_FILES})
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
                          gui++::draw
                          gui++::io
                          ${UTIL_LIBRARIES}
                          ${GUIPP_SYS_LIBRARIES})
  endif()

  GENERATE_EXPORT_HEADER(${PROJECT_NAME}
    BASE_NAME GUIPP_LOOK
    EXPORT_MACRO_NAME GUIPP_LOOK_EXPORT
    EXPORT_FILE_NAME gui++-${PROJECT_NAME}-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  if (GUIPP_STANDALONE_BUILD)
    write_subpackage_config(gui++ ${PROJECT_NAME} ctrl/${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../Config.cmake.in "${INCLUDE_FILES}")

    install(FILES ${RESOURCE_FILES}
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gui/ctrl/${PROJECT_NAME}/res)

  endif()

endif()
