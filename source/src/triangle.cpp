#include "triangle.h"
#include "glm/ext/quaternion_geometric.hpp"

Triangle::Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec3& n0, glm::vec3& n1, glm::vec3& n2)
    : m_v0(v0), m_v1(v1), m_v2(v2), m_n0(n0), m_n1(n1), m_n2(n2) { }

Triangle::Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
    : m_v0(v0), m_v1(v1), m_v2(v2)
{
    glm::vec3 e1 = v1 - v0;
    glm::vec3 e2 = v2 - v0;
    glm::vec3 normal = glm::normalize(glm::cross(e1, e2));
    m_n0 = m_n1 = m_n2 = normal; // 计算三角形的法线
}

// 计算射线与三角形的交点
// 参考：https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
std::optional<HitInfo> Triangle::intersect(const Ray &ray, float t_min, float t_max) const
{
    glm::vec3 e1 = m_v1 - m_v0; // 三角形的边e1, e1: 三角形顶点v1到三角形顶点v0的向量
    glm::vec3 e2 = m_v2 - m_v0; // 三角形的边e2, e2: 三角形顶点v2到三角形顶点v0的向量
    glm::vec3 s1 = glm::cross(ray.m_direction, e2); // s1: 射线与三角形平面的交点到三角形顶点v0的距离与e2的长度的比值
    float det = glm::dot(s1, e1); // 计算行列式, det: 射线与三角形平面的交点到三角形顶点v0的距离与e1的长度的比值

    // 添加早期判断避免除以接近0的值
    if (std::abs(det) < 1e-8)
        return {}; // 射线与三角形平面平行或几乎平行

    float inv_det = 1.0f / det; // 计算行列式的倒数, inv_det: 射线与三角形平面的交点到三角形顶点v0的距离与e1的长度的比值的倒数
    glm::vec3 s = ray.m_origin - m_v0; // 计算射线与三角形平面的交点
    float u = glm::dot(s, s1) * inv_det; // 计算u参数, u: 射线与三角形平面的交点到三角形顶点v0的距离与e1的长度的比值

    // u参数超出范围，交点不在三角形内
    if (u < 0.0f || u > 1.0f)
        return {};

    glm::vec3 s2 = glm::cross(s, e1); // 计算射线与三角形平面的交点
    float v = glm::dot(ray.m_direction, s2) * inv_det; // 计算v参数, v: 射线与三角形平面的交点到三角形顶点v0的距离与e2的长度的比值

    if (v < 0.0f || u + v > 1.0f)
        return {}; // v参数超出范围，交点不在三角形内

    float t = glm::dot(e2, s2) * inv_det; // 计算交点到射线原点的距离

    if (t > t_min && t < t_max)
    {
        glm::vec3 position = ray.m_origin + t * ray.m_direction; // 计算交点的位置,  t: 交点到射线原点的距离, ray.m_direction: 射线的方向
        glm::vec3 normal = glm::normalize(m_n0 * (1.0f - u - v) + m_n1 * u + m_n2 * v); // 计算交点的法线, 单位向量
        return HitInfo(t, position, normal);
    }

    return { };
}


