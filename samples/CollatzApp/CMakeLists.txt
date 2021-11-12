cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("CollatzApp" CXX)

add_definitions(-ftemplate-depth=110000)
guipp_sample(${PROJECT_NAME})
