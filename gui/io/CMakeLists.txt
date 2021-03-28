cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-io" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-io)

  add_subdirectory(../draw gui_draw)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  set(SOURCE_FILES
      pnm.cpp
      pnm_to_src.cpp
      wavelength_to_rgb.cpp
  )
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  add_library(gui++-io ${GUIPP_LINK} ${SOURCE_FILES} ${INCLUDE_FILES})
  add_library(gui++::io ALIAS gui++-io)

  set_target_properties(gui++-io PROPERTIES
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
    add_definitions(-Dgui___io_EXPORTS)
    target_link_libraries(gui++-io PRIVATE
                          gui++-draw
                          gui++-core
                          ${GUIPP_SYS_LIBRARIES})
  endif()

  GENERATE_EXPORT_HEADER(gui++-io
    BASE_NAME GUIPP_IO
    EXPORT_MACRO_NAME GUIPP_IO_EXPORT
    EXPORT_FILE_NAME gui++-io-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
  )

  if (GUIPP_STANDALONE_BUILD)
    set (INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/gui++/io)
    set (LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/gui++)

    target_include_directories(gui++-io
                               PUBLIC
                               "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                               "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>"
    )

    install(TARGETS gui++-io
            EXPORT gui++ioTargets
            LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
            ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    install(FILES ${INCLUDE_FILES}
            ${PROJECT_BINARY_DIR}/gui++-io-export.h
            DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    install(EXPORT gui++ioTargets
            FILE gui++ioTargets.cmake
            NAMESPACE gui++::
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in
      "${CMAKE_CURRENT_BINARY_DIR}/gui++ioConfig.cmake"
      INSTALL_DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/gui++ioConfig.cmake"
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

  endif()


endif()
