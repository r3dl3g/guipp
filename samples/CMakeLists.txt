cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("Samples" CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

add_subdirectory(./../gui gui)

if (GUIPP_USE_QT)
  message(STATUS "GUIPP samples: build with Qt5!")
  find_package(Qt5Widgets REQUIRED)
endif()

add_definitions(${GUIPP_CXX_FLAGS})
message(STATUS "samples GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
#message(STATUS "samples GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")
message(STATUS "samples GUIPP_APP_LIBRARIES: ${GUIPP_APP_LIBRARIES}")
message(STATUS "samples GUIPP_SYS_LIBRARIES: ${GUIPP_SYS_LIBRARIES}")


# add the binary tree to the search path for include files
# so that we will find Version.h
include_directories(${PROJECT_BINARY_DIR} ${GUIPP_INCLUDE_DIRS})

set(apps
    hello_world
    one_child
    WindowTest
    BorderTest
    filemanager
    list_app
    tree_app
    new_border_layout
    attach_all_layout
    dialogs
    split_view
    diagrams
    covid19
)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    add_definitions(/Y- /bigobj)
endif()

foreach(test ${apps})
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} ${GUIPP_APP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
    install(TARGETS ${test} DESTINATION bin)
    set_target_properties(${test} PROPERTIES FOLDER apps CXX_STANDARD 11)
endforeach(test)
