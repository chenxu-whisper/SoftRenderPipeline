#ifndef BASE_RENDERER_H
#define BASE_RENDERER_H

#include "camera/camera.h"
#include "shape/scene.h"
#include "util/rng.hpp"

#define DEFINE_RENDERER(Name) \
    class Name##Renderer : public BaseRenderer \
    { \
    public: \
        Name##Renderer(Camera& camera, const Scene& scene) : BaseRenderer(camera, scene) { } \
    private: \
        glm::vec3 renderPixel(const glm::ivec2& pixel_coord) override; \
    }; \

class BaseRenderer
{
public:
    BaseRenderer(Camera& camera, const Scene& scene) : m_camera(camera), m_scene(scene) { }
    void render(size_t spp, const std::filesystem::path& output_dir); // 渲染场景

private:
    virtual glm::vec3 renderPixel(const glm::ivec2& pixel_coord) = 0; // 渲染单个像素

protected:
    Camera& m_camera;
    const Scene& m_scene;
    RNG m_rng;
};

#endif //BASE_RENDERER_H
