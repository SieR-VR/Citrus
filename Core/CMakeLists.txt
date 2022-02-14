add_subdirectory(ext/SDL2)
add_subdirectory(ext/SDL_image)

add_library(Core
    camera.cc
    material.cc
    ray.cc
    renderer.cc
    scene.cc
    shader.cc
    vec.cc
    Sphere.cc
)

target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/glew/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/SDL2/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/SDL_image)

target_link_directories(Core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext/glew/lib/Release/x64)

target_link_libraries(Core
    SDL2main
    SDL2_image
    glew32
    opengl32
)
