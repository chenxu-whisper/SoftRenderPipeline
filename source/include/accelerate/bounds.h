#ifndef BOUNDS_H
#define BOUNDS_H

#include "glm/vec3.hpp"
#include "./camera/ray.h"

struct  Bounds
{
    glm::vec3 m_bmin; // Bounds的最小点
    glm::vec3 m_bmax; // Bounds的最大点

    Bounds();
    Bounds(const glm::vec3& min, const glm::vec3& max);
    void expand(const glm::vec3& point); // 扩展Bounds，确保包含给定点
    bool hasIntersection(const Ray& ray, float t_min, float t_max) const; // 判断射线是否与Bounds相交
    glm::vec3 diagonal() const; // 返回Bounds的对角线向量
};

#endif //BOUNDS_H
