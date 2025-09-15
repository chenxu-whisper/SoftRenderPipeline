#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "film.h"
#include "optional"

struct  Sphere
{
    Sphere(const glm::vec3& center, float radius);
    ~Sphere();

    glm::vec3 m_center;
    float m_radius;

    // 计算射线与球体的交点
    std::optional<float> intersect(const Ray& ray) const;
};

#endif //SPHERE_H
