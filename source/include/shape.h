#ifndef SHAPE_H
#define SHAPE_H

#include <optional>
#include "Ray.h"

struct Shape
{
    virtual ~Shape() = default;

    virtual std::optional<HitInfo> intersect(const Ray& ray,
                                             float t_min = 1e-5,
                                             float t_max = std::numeric_limits<float>::infinity()) const = 0;
};

#endif //SHAPE_H
