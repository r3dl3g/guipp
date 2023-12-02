cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

function(print_var VAR)
  message(STATUS "${PROJECT_NAME} ${VAR}: '" ${${VAR}} "'")
endfunction(print_var)

function(debug_print_var VAR)
  if(VERBOSE)
    print_var(${VAR})
  endif()
endfunction(debug_print_var)
