use crate::*;

pub struct Ray {
    pub origin: vec3::Point3,
    pub direction: vec3::Vec3,
}

impl Ray {
    pub fn at(&self, t: f32) -> vec3::Point3 {
        self.origin + self.direction * t
    }

    pub fn new(origin: vec3::Point3, direction: vec3::Vec3) -> Ray {
        Ray { origin, direction }
    }
}
