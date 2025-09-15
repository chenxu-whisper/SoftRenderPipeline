#ifndef RAY_H
#define RAY_H

# include <glm/glm.hpp>

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 hit(float t) const { return origin + t * direction; } // 计算射线与物体的交点
};

#endif //RAY_H
