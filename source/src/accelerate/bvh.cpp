#include <array>
#include <stack>
#include "./accelerate/bvh.h"

// 构建BVH树
void BVH::build(std::vector<Triangle>&& triangles)
{
    BVHTreeNode* tree_root = new BVHTreeNode(); // 创建根节点
    tree_root->m_triangles = std::move(triangles); // 将三角形移动到根节点
    tree_root->updateBounds(); // 更新根节点的边界框
    tree_root->m_depth = 1; // 设置根节点的深度为1

    recursiveSplit(tree_root); // 递归分裂根节点
    recursiveTreeToFlatten(tree_root); // 递归将树转换为扁平化数组
}

 void BVH::recursiveSplit(BVHTreeNode* tree_node)
{
    // 如果节点只有一个三角形或没有三角形，直接返回
    if (tree_node->m_triangles.size() <= 1)  return;

    // 计算节点的对角线长度和最大轴
    glm::vec3 diagonal = tree_node->m_bounds.diagonal();
    size_t axis = diagonal.x > diagonal.y ? (diagonal.x > diagonal.z ? 0 : 2) : (diagonal.y > diagonal.z ? 1 : 2);
    float split = tree_node->m_bounds.m_bmin[axis] + diagonal[axis] / 2.0f; // 计算分裂位置

    std::vector<Triangle> left_triangles;
    std::vector<Triangle> right_triangles;

    for (const auto& triangle : tree_node->m_triangles)
    {
        // 根据三角形的中心坐标判断是否进入左子节点
        if ((triangle.m_v1[axis] + triangle.m_v2[axis] + triangle.m_v0[axis]) / 3.0f < split)
            left_triangles.push_back(triangle);
        else
            right_triangles.push_back(triangle);

        // 如果左子节点或右子节点为空，直接返回
        if (left_triangles.empty() || right_triangles.empty()) return;


        BVHTreeNode* left = new BVHTreeNode(); // 创建左子节点
        BVHTreeNode* right = new BVHTreeNode(); // 创建右子节点
        tree_node->m_left = left;  // 将左子节点赋值给当前节点的左子节点
        tree_node->m_right = right; // 将右子节点赋值给当前节点的右子节点
        tree_node->m_triangles.clear(); // 清空当前节点的三角形
        tree_node->m_triangles.shrink_to_fit(); // 收缩当前节点的三角形向量
        left->m_depth = tree_node->m_depth + 1; // 设置左子节点的深度为当前节点的深度加1
        right->m_depth = tree_node->m_depth + 1; // 设置右子节点的深度为当前节点的深度加1
        left->m_triangles = std::move(left_triangles); // 将右子节点的三角形移动到左子节点
        right->m_triangles = std::move(right_triangles); // 将左子节点的三角形移动到右子节点

        left->updateBounds(); // 更新左子节点的边界框
        right->updateBounds(); // 更新右子节点的边界框
    }

    recursiveSplit(tree_node->m_left); // 递归分裂左子节点
    recursiveSplit(tree_node->m_right); // 递归分裂右子节点
}

size_t BVH::recursiveTreeToFlatten(BVHTreeNode *tree_node)
{
    // 创建一个新的BVH节点
    BVHNode bvh_node;
    bvh_node.m_bounds = tree_node->m_bounds;
    bvh_node.m_child_index = 0;
    bvh_node.m_triangle_count = tree_node->m_triangles.size();
    bvh_node.m_depth = tree_node->m_depth;

    // 将新的BVH节点添加到数组中
    size_t idx = m_bvh_nodes.size();
    m_bvh_nodes.push_back(bvh_node);

    // 如果节点没有三角形，递归遍历左右子节点
    if (bvh_node.m_triangle_count == 0)
    {
        recursiveTreeToFlatten(tree_node->m_left);
        m_bvh_nodes[idx].m_child_index = recursiveTreeToFlatten(tree_node->m_right);
    }
    else // 如果节点有三角形，将三角形添加到有序数组中
    {
        // 设置当前节点的三角形索引为有序数组的当前大小
        m_bvh_nodes[idx].m_triangle_index = m_ordered_triangles.size();
        // 将当前节点的三角形添加到有序数组中
        for (const auto& triangle : tree_node->m_triangles)
            m_ordered_triangles.push_back(triangle);
    }

    delete tree_node;

    return idx;
}

std::optional<HitInfo> BVH::intersect(const Ray &ray, float t_min, float t_max) const
{
    std::optional<HitInfo> hitInfo = std::nullopt;

    // 初始化栈，用于存储遍历路径
    std::array<int,32 > stack;
    auto ptr = stack.begin();
    size_t current_node_index = 0;

    // 遍历BVH树
    while (true)
    {
        const BVHNode& bvh_node = m_bvh_nodes[current_node_index];

        // 如果节点的边界框与射线不相交，直接跳过
        if (!bvh_node.m_bounds.hasIntersection(ray, t_min, t_max))
        {
            if (ptr == stack.begin()) break; // 如果栈为空，遍历结束
            current_node_index = *(--ptr); // 弹出栈顶元素，遍历父节点
            continue;
        }

        if (bvh_node.m_triangle_count == 0)  // 如果节点没有三角形，递归遍历左右子节点
        {
            // current_node_index++;
            // *(ptr) = bvh_node.m_child_index;
            // 先处理左子树，右子树入栈
            *ptr++ = bvh_node.m_child_index;  // 右子树索引入栈
            current_node_index++;  // 左子树索引是当前索引+1
        }
        else // 如果节点有三角形，遍历三角形
        {
            auto triangle_iter = m_ordered_triangles.begin() + bvh_node.m_triangle_index;
            for (size_t i = 0; i < bvh_node.m_triangle_count; i++)
            {
                std::optional<HitInfo> hit_info = triangle_iter->intersect(ray, t_min, t_max); // 计算三角形与射线的交点
                ++triangle_iter; // 移动到下一个三角形
                // 如果有交点，更新最近交点
                if (hit_info.has_value())
                {
                    t_max = hit_info->m_t; // 更新最近交点的参数t
                    hitInfo = hit_info; // 更新最近交点
                }
            }
            if (ptr == stack.begin()) break; // 如果栈为空，遍历结束
            current_node_index = *(--ptr); // 弹出栈顶元素，遍历父节点
        }
    }

    return hitInfo;
}
