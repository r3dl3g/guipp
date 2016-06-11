cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("Test" CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(./../gui gui)
add_subdirectory(./../ibr_std_lib ibr_std_lib)

# add the binary tree to the search path for include files
# so that we will find Version.h
# "${PROJECT_BINARY_DIR}/../IT3DCameraLib"
include_directories("${PROJECT_BINARY_DIR}" ${gui_Include} ${ibr_std_lib_Include})

link_directories(${gui_Lib}
				 ${ibr_std_lib_Lib}
				 "../boost/stage/lib")

enable_testing()

set(tests 
	WindowTest
)

foreach(test ${tests})
	add_executable(${test} WIN32 sources/${test}.cpp)
	target_link_libraries(${test} gui ibr_std_lib)
	add_test(${test} ${CMAKE_BINARY_DIR}/bin/${test})
endforeach(test)
