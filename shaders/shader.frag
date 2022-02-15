#version 460 core

#define M_PI 3.141592653589793238
#define EPSILON 0.001f

#define LAMBERTIAN 0
#define METAL 1
#define DIELECTRIC 2

struct Material {
    int type;
    vec3 albedo;
    float refIdx;
    float fuzz;
};

struct HitRecord {
    float t;
    vec3 p;
    vec3 normal;
    Material material;
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Sphere {
    vec3 position;
    float radius;
    Material material;
};

struct Camera {
    vec3 position;
    vec3 target;
    vec3 direction;

    int width;
    int height;

    float fov;
    float aspect;
    float vp_dist;

    vec3 x_direction;
    vec3 y_direction;
    vec3 look_up;

    int samples;
};

uniform Camera camera;
uniform Sphere floor_sphere;
uniform Sphere sphere1;
uniform Sphere sphere2;
uniform Sphere sphere3;

out vec4 fragColor;

float rand(float seed) {
    return fract(sin(seed * 12.9898 + 43758.5453) * 43758.5453);
}

Ray getRay(Camera camera, float x, float y) {
    vec3 center = camera.position + camera.direction * camera.vp_dist;
    float vp_width = 2. * camera.vp_dist * tan((camera.fov / 2.0) * M_PI / 180.0);
    float vp_height = vp_width * 1. / camera.aspect;

    vec3 start_pixel = center - camera.x_direction * (vp_width / 2.) - camera.y_direction * (vp_height / 2.);
    vec3 target_pixel = start_pixel + camera.x_direction * (vp_width * (x) / float(camera.width)) + camera.y_direction * (vp_height * (y) / float(camera.height));

    return Ray(camera.position, normalize(target_pixel - camera.position));
}

bool Sphere_hit(Sphere sphere, Ray ray, float t_min, float t_max, out HitRecord rec) {
    vec3 oc = ray.origin - sphere.position;
    float a = dot(ray.direction, ray.direction);
    float b = dot(oc, ray.direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = ray.origin + ray.direction * rec.t;
            rec.normal = (rec.p - sphere.position) / sphere.radius;
            rec.material = sphere.material;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = ray.origin + ray.direction * rec.t;
            rec.normal = (rec.p - sphere.position) / sphere.radius;
            rec.material = sphere.material;
            return true;
        }
    }

    return false;
}

bool dielectric_Reflect(vec3 v, vec3 n, float ni_over_nt, out vec3 refeacted) {
    vec3 uv = normalize(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
    if (discriminant > 0) {
        refeacted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    return false;
}

float dielectric_Schlick(float cosine, float ref_idx) {
    float r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
}

bool scatter(Ray ray, HitRecord rec, out vec3 attenuation, out Ray scattered, float seed) {
    if (rec.material.type == LAMBERTIAN) {
        vec3 target = rec.p + rec.normal + vec3(rand(seed), rand(seed), rand(seed));
        scattered = Ray(rec.p, normalize(target - rec.p));
        attenuation = rec.material.albedo;
        return true;
    }
    else if (rec.material.type == METAL) {
        vec3 reflected = reflect(ray.direction, rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = rec.material.albedo;
        return dot(scattered.direction, rec.normal) > 0.0;
    }
    else if (rec.material.type == DIELECTRIC) {
        vec3 outward_normal;
        vec3 reflected = reflect(ray.direction, rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cosine;

        if (dot(ray.direction, rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = rec.material.refIdx;
            cosine = rec.material.refIdx * dot(ray.direction, rec.normal) / length(ray.direction);
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / rec.material.refIdx;
            cosine = -dot(ray.direction, rec.normal) / length(ray.direction);
        }

        if (dielectric_Reflect(ray.direction, outward_normal, ni_over_nt, refracted)) {
            reflect_prob = dielectric_Schlick(cosine, rec.material.refIdx);
        }
        else {
            reflect_prob = 1.0;
        }

        if (rand(seed) < reflect_prob) {
            scattered = Ray(rec.p, reflected);
        }
        else {
            scattered = Ray(rec.p, refracted);
        }
    }
}

bool hit(Ray ray, float t_min, float t_max, out HitRecord rec) {
    HitRecord temp_rec;
    float closest_so_far = t_max;
    bool hit_anything = false;

    if (Sphere_hit(sphere1, ray, t_min, closest_so_far, temp_rec)) {
        rec = temp_rec;
        closest_so_far = temp_rec.t;
        hit_anything = true;
    }
    
    if (Sphere_hit(sphere2, ray, t_min, closest_so_far, temp_rec)) {
        rec = temp_rec;
        closest_so_far = temp_rec.t;
        hit_anything = true;
    }
    
    if (Sphere_hit(sphere3, ray, t_min, closest_so_far, temp_rec)) {
        rec = temp_rec;
        closest_so_far = temp_rec.t;
        hit_anything = true;
    }
    
    if (Sphere_hit(floor_sphere, ray, t_min, closest_so_far, temp_rec)) {
        rec = temp_rec;
        closest_so_far = temp_rec.t;
        hit_anything = true;
    }
    
    return hit_anything;
}

bool color(Ray ray, out Ray scattered, inout vec3 result, float seed) {
    HitRecord rec;
    vec3 attenuation;

    if (hit(ray, 0.0, 1.0 / 0.0, rec)) {
        if (scatter(ray, rec, attenuation, scattered, seed)) {
            result *= attenuation;
            return true;
        }
        else {
            result *= vec3(0.0);
            return true;
        }
    }
    else {
        vec3 unit_direction = normalize(ray.direction);
        float t = 0.5 * (unit_direction.y + 1.0);
        result *= (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.8, 1.0);
        return false;
    }
}

void main() {
    vec3 col = vec3(0.0);
    float random1 = rand(gl_FragCoord.y);
    float random2 = rand(gl_FragCoord.x);

    for (int i = 0; i < camera.samples; i++) {
        Ray ray = getRay(camera, gl_FragCoord.x + random1, gl_FragCoord.y + random2);
        Ray scattered;

        vec3 result = vec3(1.0);

        for (int i = 0; i < 25; i++) {
            if (!color(ray, scattered, result, random1))
                break;
            
            ray = scattered;
        }
        
        col += result;

        random1 = rand(random2);
        random2 = rand(random1);
    }

    fragColor = vec4(col / camera.samples, 1.0);
}