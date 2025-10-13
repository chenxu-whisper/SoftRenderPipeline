#ifndef PLANE_H
#define PLANE_H

#include "glm/vec3.hpp"
#include "shape.h"

struct  Plane : public Shape
{
    glm::vec3 m_origin; // 平面原点
    glm::vec3 m_direction; // 平面方向
    glm::vec3 m_normal; // 平面法线
    float m_size; // 平面大小

    Plane(glm::vec3 origin, glm::vec3 direction, glm::vec3 normal, float size);

    std::optional<HitInfo> intersect(const Ray& ray,
                                     float t_min = 1e-5,
                                     float t_max = std::numeric_limits<float>::infinity()) const override;  // 计算射线与平面的交点
};

#endif //PLANE_H
