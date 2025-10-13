#ifndef SHAPE_H
#define SHAPE_H

#include <optional>
#include "../camera/ray.h"

struct Shape
{
    virtual ~Shape() = default;

    virtual std::optional<HitInfo> intersect(const Ray& ray, float t_min, float t_max) const = 0;
};

#endif //SHAPE_H
