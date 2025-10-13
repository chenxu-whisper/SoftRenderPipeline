#include "./camera/camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(Film &film, glm::vec3 position, glm::vec3 viewpoint, float fov)
    : m_film(film), m_position(position)
{
    // 计算投影矩阵的逆矩阵
    m_clip_to_view_matrix = glm::inverse(glm::perspective(glm::radians(fov),
                                                               static_cast<float>(film.getWidth()) / static_cast<float>(film.getHeight()),
                                                               1.0f, 100.0f));

    // 计算视图矩阵的逆矩阵
    m_view_to_world_matrix = glm::inverse(glm::lookAt(m_position, viewpoint, glm::vec3(0.0f, 1.0f, 0.0f)));
}
Ray Camera::generateRay(const glm::vec2 &pixel_coord, const glm::vec2 &offset) const
{
    // ndc 坐标
    glm::vec2 ndc = (pixel_coord + offset) / glm::vec2(m_film.getWidth(), m_film.getHeight());
    ndc.y = 1.0f - ndc.y; // flip y axis
    ndc = ndc * 2.0f - 1.0f; // map to [-1, 1]

    // (x/near ,y/near, 0, 1)
    glm::vec4 clip_space = glm::vec4(ndc.x, ndc.y, 0.0f, 1.0f);
    glm::vec3 world_space = m_view_to_world_matrix * m_clip_to_view_matrix* clip_space;

    return Ray(m_position, glm::normalize(world_space - m_position));
}

Film & Camera::getFilm() const { return m_film; }
const Film & Camera::film() const { return m_film; }
Camera::~Camera() {}

