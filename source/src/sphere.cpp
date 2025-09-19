#include "sphere.h"

Sphere::Sphere(const glm::vec3 &center, float radius)
    : m_center(center), m_radius(radius)
{}

/* 计算射线与球体的交点：详见：README.md
 * @param ray 射线
 * @return 射线与球体的交点
 * @note 射线与球体的交点可能有两个，返回距离射线原点最近的那个
 *
 *  一元二次方程: a * t^2 + b * t + c = 0
 *  一元二次方程的根为：{t1, t2} = (-b ± sqrt(b^2 - 4ac)) / 2a
 *  (b^2 - 4ac) 为判别式， discriminant < 0 时，射线与球体无交点
 *  discriminant = 0 时，射线与球体相切，t1 = t2, 同一个交点
 *  discriminant > 0 时，射线与球体有两个交点
 */
std::optional<HitInfo> Sphere::intersect(const Ray &ray, float t_min, float t_max) const
{
    glm::vec3 co = ray.m_origin - m_center; // 射线与球体中心的向量
    float b = 2 * glm::dot(ray.m_direction, co); // 射线与球体中心的点积
    float c = glm::dot(co, co) - m_radius * m_radius; // 射线与球体中心的向量的模长的平方

    float discriminant = b * b - 4 * c; // 判别式
    if (discriminant < 0.0f) return std::nullopt;

    float hit_t = (-b - std::sqrt(discriminant)) * 0.5f; // 射线与球体的第一个交点
    if (hit_t < 0.0f)
        hit_t = (-b + std::sqrt(discriminant)) * 0.5f;  // 射线与球体的第二个交点
    if (hit_t > t_min && hit_t < t_max)
        return std::optional<HitInfo>(HitInfo{hit_t, ray.hit(hit_t), glm::normalize(ray.hit(hit_t) - m_center)}); // 射线与球体的交点在射线的前方

    return std::nullopt;
}

Sphere::~Sphere() {}
