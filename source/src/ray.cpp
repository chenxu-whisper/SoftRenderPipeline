# include "ray.h"

glm::vec3 Ray::hit(float t) const { return m_origin + t * m_direction; } // 计算射线与物体的交点

Ray Ray::WorldToObjectSpace(const glm::mat4& world_to_object) const
{
    // 将世界空间中的射线原点转换为物体空间中的射线原点, w 为 1 表示点向量
    glm::vec3 object_origin = world_to_object * glm::vec4(m_origin, 1.0f);
    // 将世界空间中的射线方向转换为物体空间中的射线方向, w为0表示方向向量
    glm::vec3 object_direction = world_to_object * glm::vec4(m_direction, 0.0f);

    return Ray(object_origin, object_direction);
}
