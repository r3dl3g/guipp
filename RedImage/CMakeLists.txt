cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("RedImage" CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX d)
endif()

# OpenCV Version
if(WIN32)
	set(OpenCV_DIR "${CMAKE_CURRENT_LIST_DIR}/../opencv")
    get_filename_component(CV_BASE "${OpenCV_DIR}" ABSOLUTE)
    set(CV_PATH "${CV_BASE}/include")
    set(CV_LIB_PATH "${CV_BASE}/x64/${COMPILER_VER}/lib")
    set(CV_BIN_PATH "${CV_BASE}/x64/${COMPILER_VER}/bin")

	set(Boost_DIR "${CMAKE_CURRENT_LIST_DIR}/../boost")
    get_filename_component(Boost_PATH "${Boost_DIR}" ABSOLUTE)
    set(Boost_LIB_PATH "${Boost_PATH}/stage/lib")
endif()

if (UNIX)
    set(CV_PATH "/usr/include")
    set(CV_LIB_PATH "/usr/lib")
    set(CV_BIN_PATH "/usr/bin")
endif()

add_subdirectory(./../gui gui)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

set (IncludeDirs
        ${PROJECT_BINARY_DIR}
        ${gui_include}
        ${persistent_include}
        "${CMAKE_CURRENT_SOURCE_DIR}/sources"
        "${CV_PATH}"
		"${Boost_PATH}"
)

find_package(OpenCV REQUIRED)

# add the binary tree to the search path for include files
# so that we will find Version.h

include_directories(${IncludeDirs})
link_directories(
        "${CV_LIB_PATH}"
		"${Boost_LIB_PATH}"
)

add_definitions(${gui_defines})

file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_definitions(/Y- /bigobj)
endif()

add_executable(RedImage WIN32 ${SOURCE_FILES})
target_link_libraries(RedImage ${gui_libraries} ${gui_sys_libraries})
target_link_libraries(RedImage ${OpenCV_LIBS})
