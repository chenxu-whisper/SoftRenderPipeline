#include "scene.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"

void Scene::addShape(const Shape& shape, const Material& material, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
{
    // 先将对象空间转换到世界空间
    glm::mat4 object_to_world_matrix =
        glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0f),glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::rotate(glm::mat4(1.0f),glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f),glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), scale);

    m_shape_instances.push_back({shape, material, object_to_world_matrix, glm::inverse(object_to_world_matrix)});
}

std::optional<HitInfo> Scene::intersect(const Ray &ray, float t_min, float t_max) const
{
    std::optional<HitInfo> hit_info { };
    for (const auto& shape_instance : m_shape_instances)
    {
        // 将世界空间中的射线转换为物体空间中的射线
        Ray object_ray = ray.WorldToObjectSpace(shape_instance.m_world_to_object_matrix);

        // 计算射线与物体的交点
        std::optional<HitInfo> shape_hit_info = shape_instance.m_shape.intersect(object_ray, t_min, t_max);
        if (shape_hit_info.has_value())
        {
            // 将物体空间中的交点转换为世界空间中的交点
            // 注意：交点的法线需要根据物体空间到世界空间的变换矩阵进行变换
            shape_hit_info->position = shape_instance.m_object_to_world_matrix * glm::vec4(shape_hit_info->position, 1.0f);
            shape_hit_info->normal = glm::normalize(glm::mat3(glm::transpose(shape_instance.m_world_to_object_matrix)) * shape_hit_info->normal);
            shape_hit_info->material = &shape_instance.m_material;

            // 更新最近交点
            if (!hit_info.has_value() || shape_hit_info->t < hit_info->t)
            {
                hit_info = shape_hit_info;
            }
        }
    }

    return hit_info;
}
