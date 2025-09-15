#ifndef PLANE_H
#define PLANE_H

#include "glm/vec3.hpp"
#include <optional>
#include "Ray.h"

struct  Plane
{
    glm::vec3 m_origin; // 平面原点
    glm::vec3 m_direction; // 平面方向
    glm::vec3 m_normal; // 平面法线
    float m_size; // 平面大小

    Plane(glm::vec3 origin, glm::vec3 direction, glm::vec3 normal, float size);

    std::optional<float> intersect(const Ray& ray) const;  // 计算射线与平面的交点
};

#endif //PLANE_H
