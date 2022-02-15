add_subdirectory(Core)
add_library(Assets
    Sphere.cc
    Lambertian.cc
    Metal.cc
    Dielectric.cc
)

target_link_libraries(Assets
    Core
)
