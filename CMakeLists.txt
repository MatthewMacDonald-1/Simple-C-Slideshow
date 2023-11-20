cmake_minimum_required (VERSION 3.21)
project ("Simple C Slideshow")

set(CMAKE_C_STANDARD 11) # Requires C11 standard

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/raylib ${CMAKE_BINARY_DIR}/raylib)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(src)