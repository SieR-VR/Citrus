#include <cmath>
#include <stdexcept>

#include "vec.h"

float drand48()
{
    return (float)rand() / RAND_MAX;
}

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float x, float y) : x(x), y(y) {}

float Vec2::getLength() const {
    return std::sqrt(x * x + y * y);
}

float Vec2::dot(const Vec2 &v) const {
    return x * v.x + y * v.y;
}

Vec2 Vec2::normalize() const {
    float length = getLength();
    if (length == 0) {
        throw std::runtime_error("Vec2::normalize: length is zero");
    }
    return Vec2(x / length, y / length);
}

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vec3::getLength() const {
    return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vec3 Vec3::operator+(const Vec3& v) const {
    return Vec3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vec3 Vec3::operator-(const Vec3& v) const {
    return Vec3(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vec3 Vec3::operator*(const Vec3& v) const {
    return Vec3(this->x * v.x, this->y * v.y, this->z * v.z);
}

Vec3 Vec3::operator*(float s) const {
    return Vec3(x * s, y * s, z * s);
}

Vec3 Vec3::operator/(float s) const {
    if (s == 0.f) throw std::runtime_error("Vec3::operator/: Division by zero");
    return Vec3(x / s, y / s, z / s);
}

Vec3& Vec3::operator+=(const Vec3& v) {
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& v) {
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    return *this;
}

Vec3& Vec3::operator*=(float s) {
    this->x *= s;
    this->y *= s;
    this->z *= s;
    return *this;
}

Vec3& Vec3::operator/=(float s) {
    if (s == 0.f) throw std::runtime_error("Vec3::operator/=: Division by zero");
    this->x /= s;
    this->y /= s;
    this->z /= s;
    return *this;
}

bool Vec3::operator==(const Vec3& v) const {
    return this->x == v.x && this->y == v.y && this->z == v.z;
}

bool Vec3::operator!=(const Vec3& v) const {
    return this->x != v.x || this->y != v.y || this->z != v.z;
}

float Vec3::dot(const Vec3& v) const {
    return this->x * v.x + this->y * v.y + this->z * v.z;
}

Vec3 Vec3::cross(const Vec3& v) const {
    return Vec3(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x);
}

Vec3 Vec3::normalize() const {
    float length = this->getLength();
    if (length == 0) throw std::runtime_error("Vec3::normalize: Division by zero");
    return Vec3(this->x / length, this->y / length, this->z / length);
}

Vec4::Vec4() : x(0), y(0), z(0), w(0) {}

Vec4::Vec4(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {}

float Vec4::getLength() const {
    return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
}

float Vec4::dot(const Vec4& v) const {
    return this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w;
}

Vec4 Vec4::normalize() const {
    float length = this->getLength();
    if (length == 0) throw std::runtime_error("Vec4::normalize: Division by zero");
    return Vec4(this->x / length, this->y / length, this->z / length, this->w / length);
}

Vec3 randomInUnitSphere() {
    Vec3 p;
    do {
        p = Vec3(drand48(), drand48(), drand48()) * 2.0f - Vec3(1, 1, 1);
    } while (p.dot(p) >= 1.0f);
    return p;
}

Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - n * 2.0f * v.dot(n);
}

bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) {
    Vec3 uv = v.normalize();
    float dt = uv.dot(n);
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
    if (discriminant > 0.0f) {
        refracted = (uv - n * dt) * ni_over_nt - n * std::sqrt(discriminant);
        return true;
    }
    return false;
}

float schlick(float cosine, float ref_idx) {
    float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
    r0 = r0 * r0;
    return r0 + (1-r0)*pow((1-cosine), 5);
}