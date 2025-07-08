cmake_minimum_required(VERSION 3.10)

project(
    arkanoid
    VERSION 0.0.0
    LANGUAGES CXX
)

find_package(OpenCV REQUIRED)

if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please set OpenCV_DIR.")
endif()



add_executable(test test/test.cpp)
target_include_directories(test PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(test ${OpenCV_LIBS})

file(GLOB SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(server ${SERVER_SOURCES})
target_include_directories(test PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(server ${OpenCV_LIBS})
