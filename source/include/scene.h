#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Shape.h"

struct ShapeInstance
{
    const Shape& m_shape;
    Material m_material;
    glm::mat4 m_object_to_world_matrix;
    glm::mat4 m_world_to_object_matrix;
};

struct Scene: public Shape
{
private:
    std::vector<ShapeInstance> m_shape_instances;

public:
    std::optional<HitInfo> intersect(const Ray& ray,
                                     float t_min = 1e-5,
                                     float t_max = std::numeric_limits<float>::infinity()) const override;

    void addShape(const Shape& shape,
                  const Material& material,
                  const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
                  const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f),
                  const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f));

};

#endif //SCENE_H
