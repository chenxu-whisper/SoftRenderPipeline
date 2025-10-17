#include "./accelerate/bvh.h"

// 构建BVH树
void BVH::build(std::vector<Triangle>&& triangles)
{
    m_root = new BVHNode(); // 创建根节点
    m_root->m_triangles = std::move(triangles); // 将三角形移动到根节点
    m_root->updateBounds(); // 更新根节点的边界框

    recursiveSplit(m_root); // 递归分裂根节点
}

 void BVH::recursiveSplit(BVHNode* node)
{
    // 如果节点只有一个三角形或没有三角形，直接返回
    if (node->m_triangles.size() <= 1)  return;

    glm::vec3 diagonal = node->m_bounds.diagonal();
    size_t axis = diagonal.x > diagonal.y ? (diagonal.x > diagonal.z ? 0 : 2) : (diagonal.y > diagonal.z ? 1 : 2);
    float split = node->m_bounds.m_bmin[axis] + diagonal[axis] / 2.0f; // 计算分裂位置

    std::vector<Triangle> left_triangles;
    std::vector<Triangle> right_triangles;
    for (const auto& triangle : node->m_triangles)
    {
        // 根据三角形的中心坐标判断是否进入左子节点
        if ((triangle.m_v1[axis] + triangle.m_v2[axis] + triangle.m_v0[axis]) / 3.0f < split)
            left_triangles.push_back(triangle);
        else
            right_triangles.push_back(triangle);

        // 如果左子节点或右子节点为空，直接返回
        if (left_triangles.empty() || right_triangles.empty()) return;


        BVHNode* left = new BVHNode(); // 创建左子节点
        BVHNode* right = new BVHNode(); // 创建右子节点
        node->m_left = left;  // 将左子节点赋值给当前节点的左子节点
        node->m_right = right; // 将右子节点赋值给当前节点的右子节点
        node->m_triangles.clear(); // 清空当前节点的三角形
        node->m_triangles.shrink_to_fit(); // 收缩当前节点的三角形向量
        left->m_triangles = std::move(right_triangles); // 将右子节点的三角形移动到左子节点
        right->m_triangles = std::move(left_triangles); // 将左子节点的三角形移动到右子节点
        left->updateBounds(); // 更新左子节点的边界框
        right->updateBounds(); // 更新右子节点的边界框
    }

    recursiveSplit(node->m_left); // 递归分裂左子节点
    recursiveSplit(node->m_right); // 递归分裂右子节点
}

std::optional<HitInfo> BVH::intersect(const Ray &ray, float t_min, float t_max) const
{
    std::optional<HitInfo> hitInfo = std::nullopt;
    recursiveIntersect(m_root, ray, t_min, t_max, hitInfo); // 递归遍历根节点

    return hitInfo;
}

void BVH::recursiveIntersect(BVHNode* node, const Ray& ray, float t_min, float t_max,  std::optional<HitInfo>& hitInfo) const
{
    // 如果节点的边界框与射线不相交，直接返回
    if (!node->m_bounds.hasIntersection(ray, t_min, t_max)) return;

    if (node->m_triangles.empty())  // 如果节点没有三角形，递归遍历左右子节点
    {
         recursiveIntersect(node->m_left, ray, t_min, t_max, hitInfo);
         recursiveIntersect(node->m_right, ray, t_min, t_max, hitInfo);
    }
    else // 如果节点有三角形，遍历三角形
    {
        // 遍历节点的三角形，更新最近交点
        for (const auto& triangle : node->m_triangles)
        {
            std::optional<HitInfo> hit_info = triangle.intersect(ray, t_min, t_max); // 计算三角形与射线的交点
            // 如果有交点，更新最近交点
            if (hit_info.has_value())
            {
                t_max = hit_info->m_t; // 更新最近交点的参数t
                hitInfo = hit_info; // 更新最近交点
            }
        }
    }
}
