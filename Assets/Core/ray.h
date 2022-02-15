#ifndef RAY_H_
#define RAY_H_

#include "vec.h"

class Ray
{
public:
    Ray() {}
    Ray(const Vec &origin, const Vec &direction)
        : origin(origin), direction(direction) {}

    Vec pointAtParameter(float t) const {
        return origin + direction * t;
    }

    Vec origin;
    Vec direction;
};

#endif