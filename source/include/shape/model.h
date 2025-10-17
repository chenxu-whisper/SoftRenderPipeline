#ifndef MODEL_H
#define MODEL_H

#include <filesystem>
#include <vector>
#include "triangle.h"
#include "accelerate/bvh.h"


class Model : public Shape
{
public:
    Model(const std::vector<Triangle>& triangles);
    Model(const std::filesystem::path& path);

    std::optional<HitInfo> intersect(const Ray& ray, float t_min = 0.0f, float t_max = std::numeric_limits<float>::infinity()) const override;

private:
    void buildBounds();

private:
    BVH m_bvh;
};

#endif //MODEL_H
