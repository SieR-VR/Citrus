use crate::{axis_aligned_bounding_boxes::AxisAlignedBoundingBox, *};

pub struct Sphere {
    pub center: Vec3,
    pub radius: f32,

    pub material: Box<dyn material::Material + Send + Sync>,

    pub is_moving: bool,
    pub toward: Vec3,

    pub bounding_box: Option<AxisAlignedBoundingBox>,
}

impl Sphere {
    pub fn new(center: Vec3, radius: f32, material: Box<dyn material::Material + Send + Sync>) -> Sphere {
        let rvec = Vec3::from_value(radius, radius, radius);

        Sphere {
            center,
            radius,
            material,
            is_moving: false,
            toward: Vec3::default(),
            bounding_box: Some(AxisAlignedBoundingBox::new(
                center - rvec,
                center + rvec,
            )),
        }
    }

    pub fn moving(center: Vec3, radius: f32, material: Box<dyn material::Material + Send + Sync>, toward: Vec3) -> Sphere {
        let rvec = Vec3::from_value(radius, radius, radius);

        Sphere {
            center,
            radius,
            material,
            is_moving: true,
            toward,
            bounding_box: Some(AxisAlignedBoundingBox::new(
                center - rvec,
                toward + rvec,
            )),
        }
    }

    pub fn center(&self, time: f32) -> Vec3 {
        if self.is_moving {
            self.center + self.toward * time
        } else {
            self.center
        }
    }

    pub fn bounding_box(&self, _time0: f32, _time1: f32) -> &Option<AxisAlignedBoundingBox> {
        &self.bounding_box
    }
}

impl hittable::Hittable for Sphere {
    fn hit(&self, ray: &ray::Ray, t_min: f32, t_max: f32) -> Option<hittable::HitRecord> {
        let center = self.center(ray.time);
        let oc = ray.origin - center;

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
        let outward_normal = (point - center) / self.radius;

        Some(hittable::HitRecord::new(
            point,
            ray,
            root,
            &self.material,
            &outward_normal,
        ))
    }

    fn bounding_box(&self, _time0: f32, _time1: f32) -> &Option<AxisAlignedBoundingBox> {
        &self.bounding_box
    }
}
