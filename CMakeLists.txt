cmake_minimum_required(VERSION 3.2)
project(StreamCiphers VERSION 0.1 LANGUAGES CXX)
set(CXX_STANDARD_REQUIRED 14)

include_directories(include/)

set(
  PROJECT_SOURCES
  src/main.cc
)

set(
  PROJECT_HEADERS
  include/salsa.h
  include/chacha.h
)

add_executable(
  ${CMAKE_PROJECT_NAME}
  ${PROJECT_SOURCES}
  ${PROJECT_HEADERS}
)

add_library(
  ${CMAKE_PROJECT_NAME}_lib
  ${PROJECT_SOURCES}
  ${PROJECT_HEADERS}
)

enable_testing()
add_subdirectory(test/)