#include "./renderer/normal_renderer.h"

glm::vec3 NormalRenderer::renderPixel(const glm::ivec2& pixel_coord)
{
    Ray ray = m_camera.generateRay(pixel_coord);
    std::optional<HitInfo> hit_info = m_scene.intersect(ray, 0.0f, 1000.0f);
    if (hit_info.has_value())
    {
        return hit_info->m_normal * 0.5f + glm::vec3(0.5f);
    }

    return { };
}
