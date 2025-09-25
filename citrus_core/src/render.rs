use crate::{hittable::Hittable, world::World, camera::Camera, *};

pub fn render_ray(ray: &Ray, world: &World, depth: i32, eps: f32) -> Vec3 {
    if depth <= 0 {
        return Vec3::from_value(0.0, 0.0, 0.0);
    }

    let rec = world.hit(ray, eps, f32::INFINITY);
    if let Some(rec) = rec {
        if let Some((attenuation, scattered)) = rec.material.scatter(ray, &rec) {
            render_ray(&scattered, world, depth - 1, eps) * attenuation
        } else {
            Vec3::from_value(0.0, 0.0, 0.0)
        }
    } else {
        let unit_direction = ray.direction.to_unit();
        let t = 0.5 * (unit_direction.y + 1.0);

        Vec3::from_value(1.0, 1.0, 1.0) * (1.0 - t) + Vec3::from_value(0.5, 0.7, 1.0) * t
    }
}

pub fn render_pixel(
    col: i32,
    row: i32,
    camera: &Camera,
    world: &World,
    samples_per_pixel: i32,
    max_depth: i32,
) -> Vec3 {
    let mut pixel_color = Vec3::zero();
    let inv_width = 1.0 / (camera.width - 1) as f32;
    let inv_height = 1.0 / (camera.height - 1) as f32;

    for _ in 0..samples_per_pixel {
        let ray = camera.get_ray(
            (col as f32 + rand::random::<f32>()) * inv_width,
            (row as f32 + rand::random::<f32>()) * inv_height,
        );
        pixel_color += render_ray(&ray, world, max_depth, 1e-4);
    }

    pixel_color.gamma_correction(samples_per_pixel, 2.0)
}
