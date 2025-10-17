#ifndef RAY_H
#define RAY_H

# include <glm/glm.hpp>
#include "./shape/material.h"

struct Ray
{
    glm::vec3 m_origin;
    glm::vec3 m_direction;

    glm::vec3 hit(float t) const; // 计算射线与物体的交点

    Ray WorldToObjectSpace(const glm::mat4& world_to_object) const; // 将世界空间中的射线转换为物体空间中的射线
};

struct HitInfo
{
    float m_t;
    glm::vec3 m_position;
    glm::vec3 m_normal;
    const Material *m_material = nullptr;
};

#endif //RAY_H
