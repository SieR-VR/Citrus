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
};

uniform Camera camera;
uniform Sphere sphere1;
uniform Sphere sphere2;
uniform Sphere sphere3;

out vec4 fragColor;

Ray getRay(Camera camera) {
    vec3 center = camera.position + camera.direction * camera.vp_dist;
    float vp_width = 2. * camera.vp_dist * tan((camera.fov / 2.0) * M_PI / 180.0);
    float vp_height = vp_width * 1. / camera.aspect;

    vec3 start_pixel = center - camera.x_direction * (vp_width / 2.) + camera.y_direction * (vp_height / 2.);
    vec3 target_pixel = start_pixel + camera.x_direction * (vp_width * (gl_FragCoord.x + 0.5) / float(camera.width)) - camera.y_direction * (vp_height * (gl_FragCoord.y + 0.5) / float(camera.height));

    return Ray(camera.position, normalize(target_pixel - camera.position));
}

ObjectIntersection Sphere_getIntersection(Sphere sphere, Ray ray) {
    bool hit = false;
    float dist = 0.0;
    vec3 normal = vec3(0.0);

    vec3 L = sphere.position - ray.origin;
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
    Ray ray = getRay(camera);
    ObjectIntersection closest = ObjectIntersection(false, 0.0, vec3(0.0), Material(0, vec3(0.0), vec3(0.0)));
    ObjectIntersection intersection;

    intersection = intersect(sphere1, ray);
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