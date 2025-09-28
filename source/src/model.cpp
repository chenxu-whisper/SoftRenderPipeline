#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
# include "model.h"


Model::Model(const std::vector<Triangle>& triangles) : m_triangles(triangles) {}

Model::Model(const std::filesystem::path& path)
{
    std::vector<glm::vec3> positions; // 顶点位置
    std::vector<glm::vec3> normals;  // 法线

    std::ifstream file(path);
    if (!file.is_open())
        std::cerr << "Error opening file " << path << std::endl;

    std::cout << "Loading model " << path << std::endl;

    std::string line;
    char trash; // 用于忽略行首的字符，如 'v'、'f' 等
    while (!file.eof())
    {
        std::getline(file, line);
        std::istringstream iss(line); // 字符串流，用于解析当前行

        if (line.compare(0, 2, "v ") == 0)
        {
            glm::vec3 position;
            iss>>trash>>position.x>>position.y>>position.z; // 解析顶点位置
            positions.push_back(position); // 存储顶点位置
        }
        else if (line.compare(0, 3, "vn ") == 0)
        {
            glm::vec3 normal;
            iss>>trash>>trash>>normal.x>>normal.y>>normal.z; // 解析法线
            normals.push_back(normal); // 存储法线
        }
        else if (line.compare(0, 2, "f ") == 0)
        {
            glm::ivec3 idx_v, idx_vn; // 顶点索引和法线索引
            iss>>trash; // 忽略 'f'
            // 解析顶点索引和法线索引
            iss>>idx_v.x>>trash>>trash>>idx_vn.x;
            iss>>idx_v.y>>trash>>trash>>idx_vn.y;
            iss>>idx_v.z>>trash>>trash>>idx_vn.z;
            // 索引从1开始，转换为0开始的索引
            m_triangles.push_back(Triangle(positions[idx_v.x - 1], positions[idx_v.y - 1], positions[idx_v.z - 1],
                                            normals[idx_vn.x - 1], normals[idx_vn.y - 1], normals[idx_vn.z - 1]));
        }
    }

    std::cout << "Model loaded with " << m_triangles.size() << " triangles" << std::endl;

    if (m_triangles.empty())
        std::cerr << "Warning: Model loaded but contains no triangles. Check OBJ file format." << std::endl;
}

std::optional<HitInfo> Model::intersect(const Ray& ray, float t_min, float t_max) const
{
    std::optional<HitInfo> hit; // 记录最近的交点

    // 遍历模型中的所有三角形，计算射线与每个三角形的交点
    for (const auto& triangle : m_triangles)
        {
            std::optional<HitInfo> hit_triangle = triangle.intersect(ray, t_min, t_max); // 计算射线与当前三角形的交点
            if (hit_triangle.has_value())
                if (!hit.has_value() || hit_triangle->t < hit->t) // 如果当前三角形的交点更接近射线起点，更新最近交点
                    hit = hit_triangle;
        }
    return hit;
}