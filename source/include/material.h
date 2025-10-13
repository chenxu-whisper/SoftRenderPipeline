#ifndef MATERIAL_H
#define MATERIAL_H
#include "glm/vec3.hpp"

struct Material
{
    glm::vec3 m_albedo = { 1.0f, 1.0f, 1.0f };
    bool m_is_specular = false;
    glm::vec3 m_emissive = { 0.0f, 0.0f, 0.0f };
};

#endif //MATERIAL_H
