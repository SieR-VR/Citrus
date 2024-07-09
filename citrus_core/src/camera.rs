use crate::{vec3::*, *};

#[derive(Debug, Clone, Copy)]
pub struct Camera {
    pub aspect_ratio: f32,
    pub viewport_height: f32,
    pub viewport_width: f32,

    pub v_fov: f32,

    pub lookfrom: Point3,
    pub lookat: Point3,
    pub v_up: Vec3,

    pub defocus_angle: f32,
    pub focus_dist: f32,

    viewport_u: Vec3,
    viewport_v: Vec3,

    defocus_disk_u: Vec3,
    defocus_disk_v: Vec3,

    viewport_upper_left: Point3,
}

impl Camera {
    pub fn new(aspect_ratio: f32, v_fov: f32, origin: Point3) -> Camera {
        let focus_dist = 10.0;

        let theta = v_fov.to_radians();
        let h = (theta / 2.0).tan();

        let viewport_height = 2.0 * h * focus_dist;
        let viewport_width = aspect_ratio * viewport_height;

        let lookat = Point3::from_value(0.0, 0.0, 0.0);
        let v_up = Vec3::from_value(0.0, 1.0, 0.0);

        let w = (origin - lookat).to_unit();
        let u = v_up.cross(&w).to_unit();
        let v = w.cross(&u);

        let viewport_u = u * viewport_width;
        let viewport_v = -v * viewport_height;

        let viewport_upper_left = origin - viewport_u / 2.0 - viewport_v / 2.0 - w * focus_dist;

        let defocus_angle = 0.60f32;
        let defocus_radius = focus_dist * (defocus_angle.to_radians() / 2.0).tan();
        let defocus_disk_u = u * defocus_radius;
        let defocus_disk_v = v * defocus_radius;

        Camera {
            aspect_ratio,
            viewport_height,
            viewport_width,

            v_fov,

            lookfrom: origin,
            lookat,
            v_up,

            defocus_angle,
            focus_dist,

            viewport_u,
            viewport_v,

            defocus_disk_u,
            defocus_disk_v,

            viewport_upper_left,
        }
    }

    pub fn get_ray(&self, u: f32, v: f32) -> ray::Ray {
        let pixel_sample =
            self.viewport_upper_left + self.viewport_u * u + self.viewport_v * v;

        let origin = if self.defocus_angle <= 0.0 {
            self.lookfrom
        } else {
            self.lookfrom + self.defocus_disk_sample()
        };

        let direction = (pixel_sample - origin).to_unit();

        ray::Ray::new(
            origin,
            direction,
        )
    }

    fn defocus_disk_sample(&self) -> Vec3 {
        let defocus_disk_sample = random_in_unit_disk();
        self.defocus_disk_u * defocus_disk_sample.x + self.defocus_disk_v * defocus_disk_sample.y
    }
}
