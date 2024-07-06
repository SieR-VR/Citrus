use crate::*;

#[derive(Debug, Clone, Copy)]
pub struct Camera {
    pub aspect_ratio: f32,
    pub viewport_height: f32,
    pub viewport_width: f32,

    pub focal_length: f32,

    pub origin: vec3::Point3,
    pub horizontal: vec3::Vec3,
    pub vertical: vec3::Vec3,

    pub lower_left_corner: vec3::Point3,
}

impl Camera {
    pub fn new(aspect_ratio: f32) -> Camera {
        let viewport_height = 2.0;
        let viewport_width = aspect_ratio * viewport_height;
        let focal_length = 1.0;

        let origin = vec3::Point3::from_value(0.0, 0.0, 0.0);
        let horizontal = vec3::Vec3::from_value(viewport_width, 0.0, 0.0);
        let vertical = vec3::Vec3::from_value(0.0, viewport_height, 0.0);
        let lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - vec3::Vec3::from_value(0.0, 0.0, focal_length);

        Camera {
            aspect_ratio,
            viewport_height,
            viewport_width,

            focal_length,

            origin,
            horizontal,
            vertical,

            lower_left_corner,
        }
    }

    pub fn get_ray(&self, u: f32, v: f32) -> crate::ray::Ray {
        crate::ray::Ray::new(
            self.origin,
            self.lower_left_corner + self.horizontal * u + self.vertical * v - self.origin,
        )
    }
}
