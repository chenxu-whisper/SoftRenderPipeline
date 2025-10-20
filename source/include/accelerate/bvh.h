#ifndef BVH_H
#define BVH_H

#include "./accelerate/bounds.h"
#include "./shape/shape.h"
#include "./shape/triangle.h"

// 树节点
struct BVHTreeNode
{
    Bounds m_bounds; // 节点的边界框
    std::vector<Triangle> m_triangles; // 存储当前节点的三角形
    BVHTreeNode*  m_left; // 左子节点
    BVHTreeNode*  m_right; // 右子节点
    size_t m_depth; // 节点的深度

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

// 线性BVH节点
struct  BVHNode
{
    Bounds m_bounds; // 节点的边界框
    union // 节点的子节点索引或三角形索引
    {
        int m_child_index;  // 子节点索引
        int m_triangle_index; // 三角形索引
    };
    size_t m_triangle_count; // 节点的三角形数量
    size_t m_depth; // 节点的深度
};

class BVH : public Shape
{
public:
    void build(std::vector<Triangle>&& triangles); // 构建BVH树
    std::optional<HitInfo> intersect(const Ray& ray, float t_min, float t_max) const override;  // 实现Shape的接口

private:
    void recursiveSplit(BVHTreeNode* tree_node); // 递归分裂节点
    size_t recursiveTreeToFlatten(BVHTreeNode* tree_node); // 递归将树转换为线性BVH节点

private:
    std::vector<BVHNode> m_bvh_nodes; // 线性BVH节点数组
    std::vector<Triangle> m_ordered_triangles; // 有序三角形数组
};

#endif //BVH_H
