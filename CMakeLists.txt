cmake_minimum_required(VERSION 3.10)

project(Gelato)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("${PROJECT_SOURCE_DIR}/src")

file(
    GLOB SRCS
    "${PROJECT_SOURCE_DIR}/src/*"
)

add_executable(gelato ${SRCS})
