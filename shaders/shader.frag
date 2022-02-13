#version 300 es

#ifdef GL_ES
precision highp float;
#endif

#define M_PI 3.141592653589793238
#define EPSILON 0.001f

struct Material {
    int type;
    vec3 color;
    vec3 emission;
};

struct ObjectIntersection {
    bool isHitted;
    float dist;
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

out vec4 fragColor;

uniform vec3 camera_position;
uniform vec3 camera_direction;

uniform float fov;
uniform float aspect;
uniform float vp_dist;

uniform vec3 x_direction;
uniform vec3 y_direction;

uniform Sphere sphere;
uniform Sphere sphere2;
uniform Sphere sphere3;

Ray getRay() {
    vec3 center = camera_position + camera_direction * vp_dist;
    float vp_width = 2. * vp_dist * tan((fov / 2.0) * M_PI / 180.0);
    float vp_height = vp_width * 1. / aspect;

    vec3 start_pixel = center - x_direction * (vp_width / 2.) + y_direction * (vp_height / 2.);
    vec3 target_pixel = start_pixel + x_direction * (vp_width * gl_FragCoord.x) - y_direction * (vp_height * gl_FragCoord.y);

    return Ray(camera_position, normalize(target_pixel - camera_position));
}

ObjectIntersection Sphere_getIntersection(Sphere sphere, Ray ray) {
    bool hit = false;
    float dist = 0.0;
    vec3 normal = vec3(0.0);

    vec3 L = camera_position - ray.origin;
    float tca = dot(L, ray.direction);
    float det = tca * tca - dot(L, L) + sphere.radius * sphere.radius;
    
    if (det < 0.0)
        return ObjectIntersection(hit, dist, normal, sphere.material);
    else {
        float t1 = tca - sqrt(det);
        float t2 = tca + sqrt(det);

        dist = t1 > EPSILON ? t1 : (t2 > EPSILON ? t2 : 0.0);
        if (dist != 0.0)
        {
            hit = true;
            normal = normalize(ray.origin + ray.direction * dist);
        }
    }

    return ObjectIntersection(hit, dist, normal, sphere.material);
}

ObjectIntersection intersect(Sphere sphere, Ray ray) {
    return Sphere_getIntersection(sphere, ray);
}

void main() {
    Ray ray = getRay();
    ObjectIntersection closest = ObjectIntersection(false, 0.0, vec3(0.0), Material(0, vec3(0.0), vec3(0.0)));
    ObjectIntersection intersection;

    intersection = intersect(sphere, ray);
    if (intersection.isHitted && (!closest.isHitted || intersection.dist < closest.dist))
        closest = intersection;

    intersection = intersect(sphere2, ray);
    if (intersection.isHitted && (!closest.isHitted || intersection.dist < closest.dist))
        closest = intersection;

    intersection = intersect(sphere3, ray);
    if (intersection.isHitted && (!closest.isHitted || intersection.dist < closest.dist))
        closest = intersection;

    if (closest.isHitted) {
        vec3 color = closest.material.color;
        fragColor = vec4(color, 1.0);
    }
    else
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
}