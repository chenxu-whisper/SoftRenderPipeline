#ifndef RAY_H
#define RAY_H

# include <glm/glm.hpp>

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 hit(float t) const { return origin + t * direction; }
};

#endif //RAY_H
