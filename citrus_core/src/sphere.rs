use std::rc::Rc;

use crate::*;

pub struct Sphere {
    pub center: vec3::Point3,
    pub radius: f32,

    pub material: Rc<dyn material::Material>,
}

impl hittable::Hittable for Sphere {
    fn hit(&self, ray: &ray::Ray, t_min: f32, t_max: f32) -> Option<hittable::HitRecord> {
        let oc = ray.origin - self.center;

        let a = ray.direction.length_squared();
        let half_b = oc.dot(&ray.direction);
        let c = oc.length_squared() - self.radius * self.radius;

        let discriminant = half_b * half_b - a * c;
        if discriminant < 0.0 {
            return None;
        }

        let sqrtd = discriminant.sqrt();
        let mut root = (-half_b - sqrtd) / a;

        if root < t_min || t_max < root {
            root = (-half_b + sqrtd) / a;
            if root < t_min || t_max < root {
                return None;
            }
        }

        let point = ray.at(root);
        let outward_normal = (point - self.center) / self.radius;

        Some(hittable::HitRecord::new(
            point,
            ray,
            root,
            self.material.clone(),
            &outward_normal,
        ))
    }
}
