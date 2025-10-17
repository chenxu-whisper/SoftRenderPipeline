#ifndef BVH_H
#define BVH_H

#include "./accelerate/bounds.h"
#include "./shape/shape.h"
#include "./shape/triangle.h"

struct BVHNode
{
    Bounds m_bounds; // 节点的边界框
    std::vector<Triangle> m_triangles; // 存储当前节点的三角形
    BVHNode*  m_left; // 左子节点
    BVHNode*  m_right; // 右子节点

    // 更新节点的边界框
    void updateBounds()
    {
        m_bounds = { };
        for (const auto& triangle : m_triangles)
        {
           m_bounds.expand(triangle.m_v0);
           m_bounds.expand(triangle.m_v1);
           m_bounds.expand(triangle.m_v2);
        }
    }
};

class BVH : public Shape
{
public:
    void build(std::vector<Triangle>&& triangles); // 构建BVH树
    std::optional<HitInfo> intersect(const Ray& ray, float t_min, float t_max) const override;  // 实现Shape的接口

private:
    void recursiveSplit(BVHNode* node); // 递归分裂节点
    void recursiveIntersect(BVHNode* node, const Ray& ray, float t_min, float t_max,  std::optional<HitInfo>& hitInfo) const; // 递归遍历节点

private:
    BVHNode* m_root; // 根节点
};

#endif //BVH_H
