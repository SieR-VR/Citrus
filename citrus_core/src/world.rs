use crate::*;

#[derive(Default)]
pub struct World {
    pub objects: Vec<Box<dyn hittable::Hittable + Send + Sync>>,
}

impl World {
    pub fn add(&mut self, object: Box<dyn hittable::Hittable + Send + Sync>) {
        self.objects.push(object);
    }
}

impl hittable::Hittable for World {
    fn hit(&self, ray: &Ray, t_min: f32, t_max: f32) -> Option<hittable::HitRecord> {
        let mut hit_record = None;
        let mut closest_so_far = t_max;

        for object in &self.objects {
            if let Some(record) = object.hit(ray, t_min, closest_so_far) {
                closest_so_far = record.t;
                hit_record = Some(record);
            }
        }

        hit_record
    }
}
