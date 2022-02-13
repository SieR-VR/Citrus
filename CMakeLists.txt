cmake_minimum_required(VERSION 3.0.0)
project(Citrus VERSION 0.1.0)

find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(
    ${GLEW_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)

add_executable(Citrus 
    main.cpp
    vec.cc
    ray.cc
    camera.cc
    scene.cc
    material.cc
    shader.cc
    renderer.cc
    Sphere.cc
)

target_link_libraries(
    Citrus
    GL
    GLEW::GLEW
    ${SDL2_LIBRARIES}
)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
