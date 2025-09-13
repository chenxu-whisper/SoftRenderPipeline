#ifndef CAMERA_H
#define CAMERA_H

#include "film.h"
#include "ray.h"

class Camera
{
public:
    Camera(Film& film, glm::vec3 position, glm::vec3 viewpoint, float fov);
    ~Camera();

    Ray generateRay(const glm::vec2& pixel_coord, const glm::vec2& offset = glm::vec2(0.5f)) const;

    Film& getFilm() const;
    const Film& film() const;

private:
    Film& m_film;
    glm::vec3 m_position;
    glm::mat3 m_clip_to_view_matrix;  // from clip space to view space
    glm::mat3 m_view_to_world_matrix;  // from view space to world space
};

#endif //CAMERA_H
