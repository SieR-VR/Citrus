use crate::*;

pub trait Material {
    fn scatter(&self, ray_in: &Ray, rec: &hittable::HitRecord) -> Option<(Vec3, Ray)>;
}

pub struct Lambertian {
    pub albedo: Vec3,
}

impl Material for Lambertian {
    fn scatter(&self, ray_in: &Ray, rec: &hittable::HitRecord) -> Option<(Vec3, Ray)> {
        let mut scatter_direction = rec.normal + random_unit_vector();
        if scatter_direction.near_zero() {
            scatter_direction = rec.normal;
        }

        let scattered = Ray::new(rec.point, scatter_direction, Some(ray_in.time));
        Some((self.albedo, scattered))
    }
}

pub struct Metal {
    pub albedo: Vec3,
    pub fuzz: f32,
}

impl Material for Metal {
    fn scatter(&self, ray_in: &Ray, rec: &hittable::HitRecord) -> Option<(Vec3, Ray)> {
        let reflected = reflect(&ray_in.direction.to_unit(), &rec.normal);
        let scattered = Ray::new(
            rec.point,
            reflected + random_in_unit_sphere() * self.fuzz,
            Some(ray_in.time),
        );

        Some((self.albedo, scattered))
    }
}

pub struct Dielectric {
    pub refraction_index: f32,
}

impl Material for Dielectric {
    fn scatter(&self, ray_in: &Ray, rec: &hittable::HitRecord) -> Option<(Vec3, Ray)> {
        let etai_over_etat = if rec.front_face {
            1.0 / self.refraction_index
        } else {
            self.refraction_index
        };

        let unit_direction = ray_in.direction.to_unit();
        let cos_theta = f32::min(-unit_direction.dot(&rec.normal), 1.0);
        let sin_theta = f32::sqrt(1.0 - cos_theta * cos_theta);

        let cannot_refract = etai_over_etat * sin_theta > 1.0;
        let direction =
            if cannot_refract || reflectance(cos_theta, etai_over_etat) > rand::random::<f32>() {
                reflect(&unit_direction, &rec.normal)
            } else {
                refract(&unit_direction, &rec.normal, etai_over_etat)
            };

        let scattered = Ray::new(rec.point, direction, Some(ray_in.time));

        Some((Vec3::from_value(1.0, 1.0, 1.0), scattered))
    }
}
