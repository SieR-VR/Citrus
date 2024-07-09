use std::{fs::File, io::BufWriter, path::Path, rc::Rc};

use citrus_core::{
    camera::Camera,
    material::{Dielectric, Lambertian, Material, Metal},
    render_ray::render_ray,
    sphere::Sphere,
    vec3::*,
    world::World,
};

fn main() {
    const IMAGE_WIDTH: i32 = 1280;
    const IMAGE_HEIGHT: i32 = 720;
    const SAMPLES_PER_PIXEL: i32 = 16;
    const MAX_DEPTH: i32 = 8;

    let path = Path::new("output.png");
    let file = File::create(path).unwrap();

    let ref mut writer = BufWriter::new(file);
    let mut encoder = png::Encoder::new(writer, IMAGE_WIDTH as u32, IMAGE_HEIGHT as u32);
    encoder.set_color(png::ColorType::Rgb);
    encoder.set_depth(png::BitDepth::Eight);
    encoder.set_source_gamma(png::ScaledFloat::new(1.0 / 2.2));

    let source_chromaticities = png::SourceChromaticities::new(
        (0.31270, 0.32900),
        (0.64000, 0.33000),
        (0.30000, 0.60000),
        (0.15000, 0.06000),
    );
    encoder.set_source_chromaticities(source_chromaticities);

    let mut writer = encoder.write_header().unwrap();

    let mut world = World::new();

    world.add(Box::new(Sphere {
        center: Point3::from_value(0.0, -1000.0, 0.0),
        radius: 1000.0,
        material: Rc::new(Lambertian {
            albedo: Color3::from_value(0.5, 0.5, 0.5),
        }),
    }));

    for i in -11..11 {
        for j in -11..11 {
            let material: Rc<dyn Material> = match rand::random::<f32>() {
                x if x < 0.8 => Rc::new(Lambertian {
                    albedo: Color3::from_value(
                        rand::random::<f32>() * rand::random::<f32>(),
                        rand::random::<f32>() * rand::random::<f32>(),
                        rand::random::<f32>() * rand::random::<f32>(),
                    ),
                }),
                x if x < 0.95 => Rc::new(Metal {
                    albedo: Color3::from_value(
                        0.5 * (1.0 + rand::random::<f32>()),
                        0.5 * (1.0 + rand::random::<f32>()),
                        0.5 * (1.0 + rand::random::<f32>()),
                    ),
                    fuzz: 0.5 * rand::random::<f32>(),
                }),
                _ => Rc::new(Dielectric { refraction_index: 1.5 }),
            };

            let center = Point3::from_value(
                i as f32 + 0.9 * rand::random::<f32>(),
                0.2,
                j as f32 + 0.9 * rand::random::<f32>(),
            );

            world.add(Box::new(Sphere {
                center,
                radius: 0.2,
                material: material.clone(),
            }));
        }
    }

    world.add(Box::new(Sphere {
        center: Point3::from_value(0.0, 1.0, 0.0),
        radius: 1.0,
        material: Rc::new(Dielectric { refraction_index: 1.5 }),
    }));

    world.add(Box::new(Sphere {
        center: Point3::from_value(-4.0, 1.0, 0.0),
        radius: 1.0,
        material: Rc::new(Lambertian {
            albedo: Color3::from_value(0.4, 0.2, 0.1),
        }),
    }));

    world.add(Box::new(Sphere {
        center: Point3::from_value(4.0, 1.0, 0.0),
        radius: 1.0,
        material: Rc::new(Metal {
            albedo: Color3::from_value(0.7, 0.6, 0.5),
            fuzz: 0.0,
        }),
    }));

    let camera = Camera::new(
        IMAGE_WIDTH as f32 / IMAGE_HEIGHT as f32,
        20.0,
        Point3::from_value(13.0, 2.0, 3.0),
    );

    let mut image_data: Vec<u8> = Vec::new();
    for i in 0..IMAGE_HEIGHT {
        for j in 0..IMAGE_WIDTH {
            let mut pixel_color = Color3::new();

            for _ in 0..SAMPLES_PER_PIXEL {
                let ray = camera.get_ray(
                    (j as f32 + rand::random::<f32>()) / (IMAGE_WIDTH - 1) as f32,
                    (i as f32 + rand::random::<f32>()) / (IMAGE_HEIGHT - 1) as f32,
                );
                pixel_color += render_ray(&ray, &world, MAX_DEPTH, 1e-4);
            }

            let pixel_color = pixel_color.gamma_correction(SAMPLES_PER_PIXEL, 2.0);
            image_data.extend_from_slice(pixel_color.to_pixel().as_slice());
        }
    }

    writer.write_image_data(&image_data).unwrap();
    eprintln!("Done.");
}
