cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("Gui-Test" CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

if(UNIX)
  add_definitions(-DX11 -std=c++11)
endif(UNIX)

add_subdirectory(./../gui gui)

# add the binary tree to the search path for include files
# so that we will find Version.h
# "${PROJECT_BINARY_DIR}/../IT3DCameraLib"
include_directories("${PROJECT_BINARY_DIR}" ${gui_Include} ${ibr_std_lib_Include})

link_directories(${core_Lib}
                 ${draw_Lib}
                 ${win_Lib}
                 ${ibr_std_lib_Lib}
                )

if(UNIX)
    link_directories("../boost/stage/lib")
else()
    link_directories("../boost/stage/lib")
endif(UNIX)

enable_testing()

set(tests
  WindowTest
)

foreach(test ${tests})
        add_executable(${test} WIN32 sources/${test}.cpp)
        target_link_libraries(${test} core draw win ibr_std_lib)
        if(UNIX)
          #SET_TARGET_PROPERTIES(${test} PROPERTIES PREFIX "lib")
          target_link_libraries(${test} stdc++fs X11 Xft fontconfig boost_filesystem boost_system pthread)
        endif(UNIX)
        add_test(${test} ${CMAKE_BINARY_DIR}/bin/${test})
endforeach(test)
