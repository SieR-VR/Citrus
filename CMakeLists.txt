cmake_minimum_required(VERSION 3.15.0)
project(Citrus VERSION 0.1.0)

add_executable(Citrus main.cpp)
add_subdirectory(Assets)

target_link_libraries(
    Citrus
    PUBLIC Assets
    PUBLIC Core
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
