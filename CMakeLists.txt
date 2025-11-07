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

file(GLOB CONFIG_APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/configApp/*.cpp")
add_executable(configApp ${CONFIG_APP_SOURCES})
target_compile_options(configApp PUBLIC -Wall -Wextra)
target_include_directories(configApp PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(configApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/configApp)
target_link_libraries(configApp ${OpenCV_LIBS})

file(GLOB SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/server/*.cpp")
add_executable(server ${SERVER_SOURCES})
target_compile_options(server PUBLIC -Wall -Wextra)
target_include_directories(server PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/server)
target_link_libraries(server ${OpenCV_LIBS})
