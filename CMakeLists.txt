cmake_minimum_required(VERSION 3.16)
project(SampleGen C)

set(CMAKE_C_STANDARD 99)

add_subdirectory("external/BaseLayer")

option(SAMPLE_GEN_DYNAMIC "Build SampleGen as dynamic library" ON)

file(GLOB SAMPLE_GEN_SOURCES "src/*.c")

if (SAMPLE_GEN_DYNAMIC)
  add_library(${PROJECT_NAME} DYNAMIC ${SAMPLE_GEN_SOURCES})
else()
  add_library(${PROJECT_NAME} STATIC ${SAMPLE_GEN_SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC BaseLayer)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)

option(SAMPLE_GEN_BUILD_TEST "Build SampleGenTest" OFF)

if (SAMPLE_GEN_BUILD_TEST)
  file(GLOB SAMPLE_GEN_TEST_SOURCES "test/*.c")
  add_executable(${PROJECT_NAME}Test ${SAMPLE_GEN_TEST_SOURCES})
  target_link_libraries(${PROJECT_NAME}Test ${PROJECT_NAME})
endif()
