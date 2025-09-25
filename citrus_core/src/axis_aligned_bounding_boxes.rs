use crate::{interval::Interval, *};

pub struct AxisAlignedBoundingBox([Interval; 3]);

impl AxisAlignedBoundingBox {
    pub fn new(min: Vec3, max: Vec3) -> AxisAlignedBoundingBox {
        AxisAlignedBoundingBox([
            Interval::new(min.x, max.x),
            Interval::new(min.y, max.y),
            Interval::new(min.z, max.z),
        ])
    }

    pub fn from_two_points(a: Vec3, b: Vec3) -> AxisAlignedBoundingBox {
        let min = Vec3::from_value(a.x.min(b.x), a.y.min(b.y), a.z.min(b.z));
        let max = Vec3::from_value(a.x.max(b.x), a.y.max(b.y), a.z.max(b.z));
        AxisAlignedBoundingBox::new(min, max)
    }

    pub fn hit(&self, ray: &Ray, t_min: f32, t_max: f32) -> bool {
        let mut t_min = t_min;
        let mut t_max = t_max;

        for i in 0..3 {
            let inv_d = 1.0 / ray.direction[i];
            let mut t0 = (self.0[i].min - ray.origin[i]) * inv_d;
            let mut t1 = (self.0[i].max - ray.origin[i]) * inv_d;

            if inv_d < 0.0 {
                std::mem::swap(&mut t0, &mut t1);
            }

            t_min = t0.max(t_min);
            t_max = t1.min(t_max);

            if t_max <= t_min {
                return false;
            }
        }

        true
    }
}
