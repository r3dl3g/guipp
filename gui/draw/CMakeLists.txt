cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-draw" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-draw)

  add_subdirectory(../core core)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  set(SOURCE_FILES
      arc_coords.cpp
      bitmap_info.cpp
      datamap.cpp
      converter.cpp
      frames.cpp
      drawers.cpp
      drawers_qt.cpp
      drawers_win32.cpp
      drawers_x11.cpp
      brush.cpp
      diagram.cpp
      bitmap.cpp
      font.cpp
      graphics.cpp
      image_data.cpp
      pen.cpp
      text_origin.cpp
      use.cpp
      icons.cpp
      clock.cpp
  )
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  add_library(gui++-draw ${GUIPP_LINK} ${SOURCE_FILES} ${INCLUDE_FILES})
  add_library(gui++::draw ALIAS gui++-draw)

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)

    add_definitions(-DGUIPP_BUILT_AS_STATIC)

  elseif(GUIPP_BUILD_SHARED_MODULE_LIBS)

    add_definitions(-Dgui___draw_EXPORTS)
    target_link_libraries(gui++-draw PRIVATE gui++::core ${UTIL_LIBRARIES} ${GUIPP_SYS_LIBRARIES})

  endif()

  set_target_properties(gui++-draw PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        VERSION ${GUIPP_VERSION}
                        SOVERSION ${GUIPP_VERSION_MAJOR}
                        CXX_STANDARD ${GUIPP_CXX_STANDARD}
                        )

  GENERATE_EXPORT_HEADER(gui++-draw
    BASE_NAME GUIPP_DRAW
    EXPORT_MACRO_NAME GUIPP_DRAW_EXPORT
    EXPORT_FILE_NAME gui++-draw-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
  )

  if (GUIPP_STANDALONE_BUILD)
    set (INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/gui++/draw)
    set (LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/gui++)

    target_include_directories(gui++-draw
                               PUBLIC
                               "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                               "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>"
    )

    install(TARGETS gui++-draw
            EXPORT gui++drawTargets
            LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
            ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    install(FILES ${INCLUDE_FILES}
            ${PROJECT_BINARY_DIR}/gui++-draw-export.h
            DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    install(EXPORT gui++drawTargets
            FILE gui++drawTargets.cmake
            NAMESPACE gui++::
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in
      "${CMAKE_CURRENT_BINARY_DIR}/gui++drawConfig.cmake"
      INSTALL_DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/gui++drawConfig.cmake"
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

  endif()

endif()
