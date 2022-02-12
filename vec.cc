#include <cmath>
#include <stdexcept>

#include "vec.h"

Vec3::Vec3() {}

Vec3::Vec3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

float Vec3::getLength() const {
    return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vec3 Vec3::operator+(const Vec3& v) const {
    return Vec3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vec3 Vec3::operator-(const Vec3& v) const {
    return Vec3(this->x - v.x, this->y - v.y, this->z - v.z);
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