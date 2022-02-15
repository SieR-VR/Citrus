add_library(Core
    camera.cc
    renderer.cc
    cpu_renderer.cc
    scene.cc
    shader.cc
    vec.cc
)

target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/glew/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/SDL2/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/SDL2_image/include)

target_link_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/glew/lib/Release/x64
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/SDL2/lib/x64
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/SDL2_image/lib/x64)

target_link_libraries(Core
    SDL2
    SDL2main
    SDL2_image
    glew32
    opengl32
)
