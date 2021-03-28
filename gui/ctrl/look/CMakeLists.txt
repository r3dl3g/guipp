cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-look" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-look)

  add_subdirectory(../../draw gui_draw)
  add_subdirectory(../../io gui_io)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  DebugPrint("look GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
#  message(STATUS "look GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")

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

  add_library(gui++-look ${GUIPP_LINK} ${SOURCE_FILES} ${INCLUDE_FILES} ${RESOURCE_FILES})
  add_library(gui++::look ALIAS gui++-look)

  set_target_properties(gui++-look PROPERTIES
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
    add_definitions(-Dgui___look_EXPORTS)
    target_link_libraries(gui++-look PRIVATE
                          gui++-core
                          gui++-draw
                          gui++-io
                          ${UTIL_LIBRARIES}
                          ${GUIPP_SYS_LIBRARIES})
  endif()

  GENERATE_EXPORT_HEADER(gui++-look
    BASE_NAME GUIPP_LOOK
    EXPORT_MACRO_NAME GUIPP_LOOK_EXPORT
    EXPORT_FILE_NAME gui++-look-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  if (GUIPP_STANDALONE_BUILD)
    set (INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/gui++/ctrl/look)
    set (LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/gui++)

    target_include_directories(gui++-look
                               PUBLIC
                               "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                               "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>"
    )

    install(TARGETS gui++-look
            EXPORT gui++lookTargets
            LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
            ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    install(FILES ${INCLUDE_FILES}
            ${PROJECT_BINARY_DIR}/gui++-look-export.h
            DESTINATION ${INCLUDE_INSTALL_DIR}
    )
    install(FILES ${RESOURCE_FILES}
          DESTINATION ${INCLUDE_INSTALL_DIR}/res)

    install(EXPORT gui++lookTargets
            FILE gui++lookTargets.cmake
            NAMESPACE gui++::
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/../../Config.cmake.in
      "${CMAKE_CURRENT_BINARY_DIR}/gui++lookConfig.cmake"
      INSTALL_DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/gui++lookConfig.cmake"
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

  endif()

endif()
