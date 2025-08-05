cmake_minimum_required(VERSION 3.16)
project(SampleGen C)

set(CMAKE_C_STANDARD 99)

option(SAMPLE_GEN_BUILD_TEST "Build SampleGenTest" OFF)

if (TARGET ${PROJECT_NAME})
  message(WARNING "${PROJECT_NAME} is included twice!")
else()
  add_subdirectory("external/BaseLayer")

  file(GLOB SAMPLE_GEN_SOURCES "src/*.c")

  add_library(${PROJECT_NAME} STATIC ${SAMPLE_GEN_SOURCES})
  target_link_libraries(${PROJECT_NAME} PUBLIC BaseLayer)
  target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)

  if (SAMPLE_GEN_BUILD_TEST)
    file(GLOB SAMPLE_GEN_TEST_SOURCES "test/*.c")
    add_executable(${PROJECT_NAME}Test ${SAMPLE_GEN_TEST_SOURCES})
    target_link_libraries(${PROJECT_NAME}Test ${PROJECT_NAME})
  endif()
endif()


