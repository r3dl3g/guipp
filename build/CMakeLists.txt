cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("info" CXX)

function(print_var VAR)
    message(STATUS "${VAR} ${${VAR}}")
endfunction(print_var)

print_var(CMAKE_CXX_PLATFORM_ID)
print_var(CMAKE_CXX_COMPILER_ID)
print_var(CMAKE_CXX_COMPILER_VERSION)
