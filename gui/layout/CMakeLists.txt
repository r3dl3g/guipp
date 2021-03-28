cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-layout" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-layout)

  add_subdirectory(../win win)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  set(SOURCE_FILES
      attach_layout.cpp
      layout.cpp
  )
  set(INCLUDE_FILES
    adaption_layout.h
    attach_layout.h
    border_layout.h
    dynamic_border_layout.h
    grid_layout.h
    layout_container.h
    layout.h
    lineup_layout.h
    weighted_layout.h
    adaption_layout.inl
    attach_layout.inl
    border_layout.inl
    dynamic_border_layout.inl
    grid_layout.inl
    layout_container.inl
    layout.inl
    lineup_layout.inl
    weighted_layout.inl
  )

  add_library(gui++-layout ${GUIPP_LINK} ${SOURCE_FILES} ${INCLUDE_FILES})
  add_library(gui++::layout ALIAS gui++-layout)

  set_target_properties(gui++-layout PROPERTIES
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
    add_definitions(-Dgui___layout_EXPORTS)
    target_link_libraries(gui++-layout PRIVATE
                          gui++::core
                          gui++::win
                          ${GUIPP_SYS_LIBRARIES})
  endif()

  GENERATE_EXPORT_HEADER(gui++-layout
    BASE_NAME GUIPP_LAYOUT
    EXPORT_MACRO_NAME GUIPP_LAYOUT_EXPORT
    EXPORT_FILE_NAME gui++-layout-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
  )

  if (GUIPP_STANDALONE_BUILD)
    set (INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/gui++/layout)
    set (LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/gui++)

    target_include_directories(gui++-layout
                               PUBLIC
                               "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                               "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>"
    )

    install(TARGETS gui++-layout
            EXPORT gui++layoutTargets
            LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
            ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    install(FILES ${INCLUDE_FILES}
            ${PROJECT_BINARY_DIR}/gui++-layout-export.h
            DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    install(EXPORT gui++layoutTargets
            FILE gui++layoutTargets.cmake
            NAMESPACE gui++::
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in
      "${CMAKE_CURRENT_BINARY_DIR}/gui++layoutConfig.cmake"
      INSTALL_DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/gui++layoutConfig.cmake"
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

  endif()

endif()
