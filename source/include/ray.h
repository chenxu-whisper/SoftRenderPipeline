#ifndef RAY_H
#define RAY_H

# include <glm/glm.hpp>

struct Ray
{
    glm::vec3 m_origin;
    glm::vec3 m_direction;

    glm::vec3 hit(float t) const; // 计算射线与物体的交点

    Ray WorldToObjectSpace(const glm::mat4& world_to_object) const; // 将世界空间中的射线转换为物体空间中的射线
};

struct HitInfo
{
    float t;
    glm::vec3 position;
    glm::vec3 normal;
};

#endif //RAY_H
