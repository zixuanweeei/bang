cmake_minimum_required(VERSION 3.9)
message(STATUS "cmake version: ${CMAKE_VERSION}")

set(XTOP_PROJECT "XTOP")
set(XTOP_NAME "xtop")
set(XTOP_VERSION 0.0.1)

project(${XTOP_PROJECT} VERSION ${XTOP_VERSION} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(3rdparty/level-zero)

add_executable(xtop tests/l0/test_show_device_info.cc)

target_include_directories(xtop
  PUBLIC
  ${PROJECT_SOURCE_DIR}/3rdparty/level-zero/include
  ${PROJECT_BINARY_DIR}/3rdparty/level-zero/include
  )

target_link_libraries(xtop
  PRIVATE
  ze_loader
  )
