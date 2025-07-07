cmake_minimum_required(VERSION 3.10)

project(
    arkanoid
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNT1IME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)

if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please set OpenCV_DIR.")
endif()

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(test src/test.cpp)
target_link_libraries(test ${OpenCV_LIBS})

add_executable(server src/server.cpp)
target_link_libraries(server ${OpenCV_LIBS})
