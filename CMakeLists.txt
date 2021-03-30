cmake_minimum_required(VERSION 3.5)

project(Arduino_Media_Control LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)

add_executable(Arduino_Media_Control
  main.cpp
)

qt5_use_modules(Arduino_Media_Control Core SerialPort)

target_link_libraries(Arduino_Media_Control Qt5::Core)
