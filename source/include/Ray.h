#ifndef RAY_H
#define RAY_H

# include <glm/glm.hpp>

struct Ray
{
    glm::vec3 m_origin;
    glm::vec3 m_direction;

    glm::vec3 hit(float t) const { return m_origin + t * m_direction; } // 计算射线与物体的交点
};

struct HitInfo
{
    float t;
    glm::vec3 position;
    glm::vec3 normal;
};

#endif //RAY_H
