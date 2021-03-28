cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-core" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-core)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

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

  DebugPrint("GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")

  add_library(gui++-core ${GUIPP_LINK} ${SOURCE_FILES} ${INCLUDE_FILES})
  add_library(gui++::core ALIAS gui++-core)

  set_target_properties(gui++-core PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    FOLDER libraries
    VERSION ${GUIPP_VERSION}
    SOVERSION ${GUIPP_VERSION_MAJOR}
    CXX_STANDARD ${GUIPP_CXX_STANDARD}
  )

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    target_link_libraries(gui++-core PRIVATE ${LOGGING_LIBRARIES})

  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___core_EXPORTS)
    target_link_libraries(gui++-core PRIVATE
                          ${LOGGING_LIBRARIES}
                          ${GUIPP_SYS_LIBRARIES}
                          ${LOGGING_SYS_LIBRARIES})
  endif()

  GENERATE_EXPORT_HEADER(gui++-core
    BASE_NAME GUIPP_CORE
    EXPORT_MACRO_NAME GUIPP_CORE_EXPORT
    EXPORT_FILE_NAME gui++-core-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC)

  if (GUIPP_STANDALONE_BUILD)
    set (INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/gui++/core)
    set (LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/gui++)

    target_include_directories(gui++-core
                               PUBLIC
                               "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                               "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>")

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in
                                  "${CMAKE_CURRENT_BINARY_DIR}/gui++coreConfig.cmake"
                                  INSTALL_DESTINATION ${LIBRARY_INSTALL_DIR}/cmake)

    install(TARGETS gui++-core
            EXPORT gui++coreTargets
            LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
            ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR})

    install(EXPORT gui++coreTargets
            FILE gui++coreTargets.cmake
            NAMESPACE gui++::
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake)

    install(FILES ${INCLUDE_FILES}
            ${PROJECT_BINARY_DIR}/gui++-core-export.h
            DESTINATION ${INCLUDE_INSTALL_DIR})

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/gui++coreConfig.cmake"
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake)

  endif()

endif()
