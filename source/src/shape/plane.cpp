#include "./shape/plane.h"

Plane::Plane(glm::vec3 origin, glm::vec3 direction, glm::vec3 normal, float size)
    : m_origin(origin), m_direction(direction), m_normal(normal), m_size(size)
{
    // 确保direction与normal正交
    m_direction = glm::normalize(direction - glm::dot(direction, normal) * normal);
}

// 计算射线与平面的交点，且控制平面大小（矩形）
std::optional<HitInfo> Plane::intersect(const Ray &ray, float t_min, float t_max) const
{
    // 计算射线与无限大平面的交点
    float denominator = glm::dot(ray.m_direction, m_normal);
    // 检查射线是否平行于平面
    if (std::abs(denominator) < 1e-6f) return std::nullopt;

    // 计算射线与平面的交点
    float t = glm::dot(m_origin - ray.m_origin, m_normal) / denominator;
    if (t < 0.0f) return std::nullopt;

    // 计算交点坐标
    glm::vec3 hit_point = ray.hit(t);
    // 计算交点相对于平面原点的向量
    glm::vec3 point_to_origin = hit_point - m_origin;
    // 确保m_direction与m_normal正交
    glm::vec3 dir = glm::normalize(m_direction - glm::dot(m_direction, m_normal) * m_normal);
    // 计算另一个正交向量
    glm::vec3 other_dir = glm::cross(m_normal, dir);
    // 计算交点在这两个方向上的投影长度
    float proj1 = glm::dot(point_to_origin, dir);
    float proj2 = glm::dot(point_to_origin, other_dir);
    // 检查交点是否在矩形范围内（以原点为中心，边长为m_size的正方形）
    if (std::abs(proj1) > m_size / 2.0f || std::abs(proj2) > m_size / 2.0f) {
        return std::nullopt;
    }

    return HitInfo{t, hit_point, m_normal};
}