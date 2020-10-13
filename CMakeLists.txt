cmake_minimum_required(VERSION 3.9)
message(STATUS "cmake version: ${CMAKE_VERSION}")

set(BANG_PROJECT "BANG")
set(BANG_NAME "bang")
set(BANG_VERSION 0.0.1)

project(${BANG_PROJECT} VERSION ${BANG_VERSION} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(3rdparty/level-zero)

add_executable(bang tests/l0/test_show_device_info.cc)

target_include_directories(bang
  PUBLIC
  ${PROJECT_SOURCE_DIR}/3rdparty/level-zero/include
  ${PROJECT_BINARY_DIR}/3rdparty/level-zero/include
  )

target_link_libraries(bang
  PRIVATE
  ze_loader
  )
