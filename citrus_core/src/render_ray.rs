use crate::{hittable::Hittable, ray::Ray, vec3::Vec3, world::World};

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
