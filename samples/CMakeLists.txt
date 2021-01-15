cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("Samples" CXX)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

add_subdirectory(./../gui gui)

if (GUIPP_USE_QT)
  DebugPrint("GUIPP samples: build with Qt5!")
  find_package(Qt5Widgets REQUIRED)
#  find_package(ICU REQUIRED data i18n io lx uc)
#  find_package(PNG REQUIRED)
#  set(APP_SYS_LIBRARIES ${ICU_LIBRARIES} ${PNG_LIBRARIES})
endif()

if (GUIPP_USE_BOOST)
  find_package(Boost 1.66 REQUIRED COMPONENTS filesystem)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} "/Y- /bigobj")
endif()

add_definitions(${GUIPP_CXX_FLAGS})
DebugPrint("samples GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
#DebugPrint("samples GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")
DebugPrint("samples GUIPP_APP_LIBRARIES: ${GUIPP_APP_LIBRARIES}")
DebugPrint("samples GUIPP_SYS_LIBRARIES: ${GUIPP_SYS_LIBRARIES}")


# add the binary tree to the search path for include files
# so that we will find Version.h
include_directories(${PROJECT_BINARY_DIR} ${GUIPP_INCLUDE_DIRS})

set(apps
    hello_world
    pnm2src
    one_child
    new_border_layout
    attach_all_layout
    look_and_feel
    diagrams
    dialogs
    split_view
    list_app
    tree_app
    filemanager
    covid19
    WindowTest
    BorderTest
)

add_definitions(${GUIPP_CXX_FLAGS})
if (${CMAKE_VERSION} VERSION_LESS "3.13.0")
  set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${GUIPP_EXE_LINKER_FLAGS})
else ()
  add_link_options(${GUIPP_EXE_LINKER_FLAGS})
endif ()

foreach(app ${apps})
    add_executable(${app} WIN32 ${app}.cpp)
#    target_link_libraries(${app} ${APP_SYS_LIBRARIES} ${GUIPP_APP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
    target_link_libraries(${app} ${GUIPP_APP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
    install(TARGETS ${app} DESTINATION bin)
    set_target_properties(${app} PROPERTIES
                          FOLDER apps
                          CXX_STANDARD ${GUIPP_CXX_STANDARD})
endforeach(app)
