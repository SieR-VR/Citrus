cmake_minimum_required(VERSION 3.0.0)
project(Citrus VERSION 0.1.0)

add_subdirectory(libspng-0.7.2)
add_executable(Citrus 
    main.cpp
    vec.cc
    ray.cc
    camera.cc
    scene.cc
    material.cc
    renderer.cc
    Sphere.cc
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
