cmake_minimum_required(VERSION 3.10)
project(sofa_project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)  # Set build type to Debug
set(CMAKE_CXX_FLAGS_DEBUG "-g")  # Enable debug symbols

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
    external/lumina/include
    external/lumina/libs
    ${SDL2_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

add_executable(main source/main.cpp)

target_link_libraries(
    main 
    ${SDL2_LIBRARIES}
    ${GLEW_LIBRARIES}
    GL
)

add_executable(slice source/slice.cpp)

target_link_libraries(
    slice
    ${SDL2_LIBRARIES}
    ${GLEW_LIBRARIES}
    GL
)

add_executable(multi_window source/multi_window.cpp)

target_link_libraries(
    multi_window
    ${SDL2_LIBRARIES}
    ${GLEW_LIBRARIES}
    GL
)