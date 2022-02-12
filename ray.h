#ifndef RAY_H_
#define RAY_H_

#include "vec.h"

class Ray {
public:
    Vec origin;
    Vec direction;
    Ray(const Vec& origin, const Vec& direction);
};

#endif