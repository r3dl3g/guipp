cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("info" CXX)

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    set(_variableValue ${${_variableName}})
    string(FIND "${_variableValue}" "\n" value)
    if(${value} EQUAL -1)
      message(STATUS "${_variableName} ${_variableValue}")
    endif()
endforeach()
