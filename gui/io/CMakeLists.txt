cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-io" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-io-obj)

    add_subdirectory(../draw gui_draw)

    set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
    set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
    include_directories(${GUIPP_INCLUDE_DIRS})

    if (WIN32)
      add_definitions(-DWIN32_LEAN_AND_MEAN)
    endif (WIN32)

    add_definitions(${GUIPP_CXX_FLAGS})

    set (INCLUDE_INSTALL_DIR include/guipp/gui/io)
    set (LIBRARY_INSTALL_DIR lib/guipp)

    file(GLOB SOURCE_FILES "*.cpp")
    file(GLOB INCLUDE_FILES "*.h" "*.inl")

    add_library(gui++-io-obj OBJECT ${SOURCE_FILES} ${INCLUDE_FILES})
    set_target_properties(gui++-io-obj PROPERTIES
                          POSITION_INDEPENDENT_CODE ON
                          FOLDER libraries
                          CXX_STANDARD 11
    )

    if(GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_definitions(-DGUIPP_BUILT_AS_STATIC)
        add_library(gui++-io-static STATIC $<TARGET_OBJECTS:gui++-io-obj>)
        install(TARGETS gui++-io-static DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()

    if(GUIPP_BUILD_SHARED_MODULE_LIBS)
        add_definitions(-Dgui___io_obj_EXPORTS)
        add_library(gui++-io SHARED $<TARGET_OBJECTS:gui++-io-obj>)
        target_link_libraries(gui++-io PRIVATE gui++-draw gui++-core ${GUIPP_SYS_LIBRARIES})
        set_target_properties(gui++-io PROPERTIES
                                       POSITION_INDEPENDENT_CODE ON
                                       FOLDER libraries
                                       VERSION ${gui_VERSION}
                                       SOVERSION ${gui_VERSION_MAJOR}
                                       CXX_STANDARD 11
        )
        install(TARGETS gui++-io DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()

    GENERATE_EXPORT_HEADER(gui++-io-obj
        BASE_NAME GUIPP_IO
        EXPORT_MACRO_NAME GUIPP_IO_EXPORT
        EXPORT_FILE_NAME gui++-io-export.h
        STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

    install(FILES ${INCLUDE_FILES}
                  ${PROJECT_BINARY_DIR}/gui++-io-export.h
            DESTINATION ${INCLUDE_INSTALL_DIR})


endif()
