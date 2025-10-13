#ifndef SPHERE_H
#define SPHERE_H

#include "../camera/film.h"
#include "shape.h"

struct  Sphere : public Shape
{
    Sphere(const glm::vec3& center, float radius);
    ~Sphere();

    glm::vec3 m_center;
    float m_radius;

    // 计算射线与球体的交点
    std::optional<HitInfo> intersect(const Ray& ray,
                                     float t_min = 1e-5,
                                     float t_max = std::numeric_limits<float>::infinity()) const override;
};

#endif //SPHERE_H
