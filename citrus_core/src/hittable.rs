use std::rc::Rc;

use crate::*;

pub struct HitRecord {
    pub point: vec3::Point3,
    pub normal: vec3::Vec3,
    pub t: f32,

    pub material: Rc<dyn material::Material>,

    pub front_face: bool,
}

impl HitRecord {
    pub fn new(
        point: vec3::Point3,
        ray: &ray::Ray,
        t: f32,
        material: Rc<dyn material::Material>,
        outward_normal: &vec3::Vec3,
    ) -> HitRecord {
        let front_face = ray.direction.dot(outward_normal) < 0.0;
        let normal = if front_face {
            *outward_normal
        } else {
            -*outward_normal
        };

        HitRecord {
            point,
            normal,
            t,
            material,
            front_face,
        }
    }
}

pub trait Hittable {
    fn hit(&self, ray: &ray::Ray, t_min: f32, t_max: f32) -> Option<HitRecord>;
}
