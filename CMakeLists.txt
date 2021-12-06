cmake_minimum_required(VERSION 3.20)
project(911)

set(CMAKE_CXX_STANDARD 17)

add_executable(911 main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR libs/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML COMPONENTS graphics window system audio network REQUIRED)

target_link_libraries(911 sfml-graphics sfml-window sfml-system)