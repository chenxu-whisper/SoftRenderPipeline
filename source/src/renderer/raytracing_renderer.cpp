#include "renderer/raytracing_renderer.h"
#include "./util/frame.h"

glm::vec3 RaytracingRenderer::renderPixel(const glm::ivec2& pixel_coord)
{
    Ray ray = m_camera.generateRay(glm::vec2(pixel_coord), glm::vec2(m_rng.uniform(), m_rng.uniform())); // 生成射线
    glm::vec3 beta = { 1.0f, 1.0f, 1.0f }; // 反射系数
    glm::vec3 final_color = { 0.0f, 0.0f, 0.0f }; // 最终颜色

    // 光线追踪：https://raytracing.github.io/books/RayTracingInOneWeekend.html
    while (true)
    {
        std::optional<HitInfo> hit_info = m_scene.intersect(ray, 0.0f, 1000.0f); // 计算射线与场景中物体的交点
        if (hit_info.has_value())
        {
            final_color += beta * hit_info->material->m_emissive;
            beta *= hit_info->material->m_albedo; //

            ray.m_origin = hit_info->position + hit_info->normal * 0.001f; // 避免与物体本身相交
            Frame frame(hit_info->normal); // 计算反射方向
            glm::vec3 light_direction;
            if (hit_info->material->m_is_specular)
            {
                glm::vec3 view_dir = frame.world_to_screen(-ray.m_direction); // 计算反射方向
                light_direction = glm::vec3(-view_dir.x, view_dir.y, -view_dir.z); // 与反射方向相反
            }
            else
            {
                // 随机采样一个方向
                do{
                    light_direction = glm::vec3(m_rng.uniform(), m_rng.uniform(), m_rng.uniform()); // 生成随机方向
                    light_direction = light_direction * 2.0f  - glm::vec3(1.0f); // 将随机方向映射到[-1, 1]范围
                } while (glm::length(light_direction) > 1.0f); // 确保方向在单位球内

                // 半球采样
                if (light_direction.y < 0.0f)
                    light_direction.y = -light_direction.y;
            }
            ray.m_direction = frame.world_to_screen(light_direction); // 转换为屏幕空间
        }
        else { break; }
    }

    return final_color;
}
