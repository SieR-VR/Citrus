use crate::*;

pub struct Ray {
    pub origin: Vec3,
    pub direction: Vec3,

    pub time: f32,
}

impl Ray {
    pub fn at(&self, t: f32) -> Vec3 {
        self.origin + self.direction * t
    }

    pub fn new(origin: Vec3, direction: Vec3, time: Option<f32>) -> Ray {
        Ray { origin, direction, time: time.unwrap_or(0.0) }
    }
}
