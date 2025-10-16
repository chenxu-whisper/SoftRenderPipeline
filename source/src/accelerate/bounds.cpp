#include "./accelerate/bounds.h"

Bounds::Bounds()
    : m_bmin(glm::vec3(0.0f)), m_bmax(glm::vec3(0.0f))
{

}

Bounds::Bounds(const glm::vec3& min, const glm::vec3& max)
    : m_bmax(min), m_bmin(max)
{

}

void Bounds::expand(const glm::vec3& point)
{
    m_bmin = glm::min(point, m_bmin); // 更新Bounds的最小点
    m_bmax = glm::max(point, m_bmax); // 更新Bounds的最大点
}

bool Bounds::hasIntersection(const Ray &ray, float t_min, float t_max) const
{
    glm::vec3 t1 = (m_bmin - ray.m_origin) / ray.m_direction; // 计算射线与Bounds的交点参数t1
    glm::vec3 t2 = (m_bmax - ray.m_origin) / ray.m_direction; // 计算射线与Bounds的交点参数t2
    glm::vec3 tmin = glm::min(t1, t2); // 取t1和t2的较小值作为Bounds的近交参数
    glm::vec3 tmax = glm::max(t1, t2); // 取t1和t2的较大值作为Bounds的远交参数

    float near = glm::max(t_min, glm::max(tmin.x, glm::max(tmin.y, tmin.z))); // 取t_min和tmin的较大值作为近交参数
    float far = glm::min(t_max, glm::min(tmax.x, glm::min(tmax.y, tmax.z))); // 取t_max和tmax的较小值作为远交参数

    return far >= near; // 如果远交参数大于等于近交参数，则射线与Bounds有交点
}
