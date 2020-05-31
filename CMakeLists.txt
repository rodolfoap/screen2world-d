cmake_minimum_required(VERSION 3.10)
project(gen-points)

# Force C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Equivalent to (CFLAGS) -g
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "Binary gen-points will go to: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Equivalent to (CFLAGS) -g
set(CMAKE_C_FLAGS "-g")

# OPENCV ####################################
find_package(OpenCV REQUIRED)

add_executable(gen-points gen-points.cpp autocalib.cpp yamlgen.cpp screen2worldK.cpp)
target_compile_definitions(gen-points PRIVATE DEBUG=false)

include_directories(${OpenCV_INCLUDE_DIRS})
target_include_directories(gen-points PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(gen-points ${OpenCV_LIBS} stdc++fs)
