use crate::*;

pub trait Material {
    fn scatter(
        &self,
        ray_in: &ray::Ray,
        rec: &hittable::HitRecord,
    ) -> Option<(vec3::Color3, ray::Ray)>;
}

pub struct Lambertian {
    pub albedo: vec3::Color3,
}

impl Material for Lambertian {
    fn scatter(
        &self,
        _ray_in: &ray::Ray,
        rec: &hittable::HitRecord,
    ) -> Option<(vec3::Color3, ray::Ray)> {
        let mut scatter_direction = rec.normal + random_unit_vector();
        if scatter_direction.near_zero() {
            scatter_direction = rec.normal;
        }

        let scattered = ray::Ray {
            origin: rec.point,
            direction: scatter_direction,
        };

        Some((self.albedo, scattered))
    }
}

pub struct Metal {
    pub albedo: vec3::Color3,
    pub fuzz: f32,
}

impl Material for Metal {
    fn scatter(
        &self,
        ray_in: &ray::Ray,
        rec: &hittable::HitRecord,
    ) -> Option<(vec3::Color3, ray::Ray)> {
        let reflected = reflect(&ray_in.direction.to_unit(), &rec.normal);
        let scattered = ray::Ray {
            origin: rec.point,
            direction: reflected + random_in_unit_sphere() * self.fuzz,
        };

        Some((self.albedo, scattered))
    }
}

pub struct Dielectric {
    pub refraction_index: f32,
}

impl Material for Dielectric {
    fn scatter(
        &self,
        ray_in: &ray::Ray,
        rec: &hittable::HitRecord,
    ) -> Option<(vec3::Color3, ray::Ray)> {
        let etai_over_etat = if rec.front_face {
            1.0 / self.refraction_index
        } else {
            self.refraction_index
        };

        let unit_direction = ray_in.direction.to_unit();
        let refracted = refract(&unit_direction, &rec.normal, etai_over_etat);

        let scattered = ray::Ray {
            origin: rec.point,
            direction: refracted,
        };

        Some((vec3::Color3::from_value(1.0, 1.0, 1.0), scattered))
    }
}
