#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

struct Triangle : public Shape
{
    glm::vec3 m_v0, m_v1, m_v2;
    glm::vec3 m_n0, m_n1, m_n2;

    Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec3& n0, glm::vec3& n1, glm::vec3& n2);
    Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);

    std::optional<HitInfo> intersect(const Ray &ray, float t_min = 1e-5, float t_max = std::numeric_limits<float>::infinity()) const override;

};

#endif //TRIANGLE_H
