#ifndef VEC_H_
#define VEC_H_

class Vec3 {
public:
    float x, y, z;
    Vec3();
    Vec3(float x, float y, float z);
    float getLength() const;
    Vec3 operator+(const Vec3& v) const;
    Vec3 operator-(const Vec3& v) const;
    Vec3 operator*(float s) const;
    Vec3 operator/(float s) const;
    Vec3 operator-() const;
    Vec3& operator+=(const Vec3& v);
    Vec3& operator-=(const Vec3& v);
    Vec3& operator*=(float s);
    Vec3& operator/=(float s);
    bool operator==(const Vec3& v) const;
    bool operator!=(const Vec3& v) const;
    float dot(const Vec3& v) const;
    Vec3 cross(const Vec3& v) const;
    Vec3 normalize() const;
};

typedef Vec3 Color;
typedef Vec3 Point;
typedef Vec3 Vec;

#endif