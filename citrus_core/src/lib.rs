pub mod vec3;
pub mod ray;

pub mod hittable;
pub mod sphere;

pub mod material;

pub mod camera;
pub mod world;

pub mod render_ray;

pub fn random_unit_vector() -> vec3::Vec3 {
    let a = rand::random::<f32>() * 2.0 * std::f32::consts::PI;
    let z = rand::random::<f32>() * 2.0 - 1.0;
    let r = (1.0 - z * z).sqrt();

    vec3::Vec3 { x: r * a.cos(), y: r * a.sin(), z }
}

pub fn random_in_unit_sphere() -> vec3::Vec3 {
    loop {
        let p = vec3::Vec3::random() * 2.0 - vec3::Vec3::from_value(1.0, 1.0, 1.0);
        if p.length_squared() < 1.0 {
            return p;
        }
    }
}

pub fn random_in_unit_disk() -> vec3::Vec3 {
    loop {
        let p = vec3::Vec3::from_value(rand::random::<f32>(), rand::random::<f32>(), 0.0) * 2.0 - vec3::Vec3::from_value(1.0, 1.0, 0.0);
        if p.length_squared() < 1.0 {
            return p;
        }
    }
}

pub fn reflect(v: &vec3::Vec3, n: &vec3::Vec3) -> vec3::Vec3 {
    *v - *n * v.dot(n) * 2.0
}

pub fn refract(uv: &vec3::Vec3, n: &vec3::Vec3, etai_over_etat: f32) -> vec3::Vec3 {
    let cos_theta = (-*uv).dot(n).min(1.0);
    let r_out_perp = (*uv + *n * cos_theta) * etai_over_etat;
    let r_out_parallel = *n * -(1.0 - r_out_perp.length_squared()).abs().sqrt();

    r_out_perp + r_out_parallel
}

pub fn reflectance(cosine: f32, ref_idx: f32) -> f32 {
    let r0 = ((1.0 - ref_idx) / (1.0 + ref_idx)).powi(2);
    r0 + (1.0 - r0) * (1.0 - cosine).powi(5)
}
