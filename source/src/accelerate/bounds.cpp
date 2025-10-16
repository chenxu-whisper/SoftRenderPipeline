#include "./accelerate/bounds.h"

Bounds::Bounds()
    : m_bmin(std::numeric_limits<float>::infinity()), m_bmax(-std::numeric_limits<float>::infinity()) // 初始化Bounds的最小点和最大点为无穷大和负无穷大
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

    float near = glm::max(tmin.x, glm::max(tmin.y, tmin.z)); // 取tmin的较大值作为近交参数
    float far = glm::min(tmax.x, glm::min(tmax.y, tmax.z)); // 取tmax的较小值作为远交参数

    // 如果近交参数大于远交参数，或者远交参数小于等于近交参数，射线与Bounds没有交点
    if (near<=t_min && far>=t_max)  return  false;

    // 如果近交参数小于等于远交参数，射线与Bounds有交点
    return glm::max(near,t_min) <= glm::min(far,t_max);
}
