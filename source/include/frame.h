#ifndef FRAME_H
#define FRAME_H
#include "glm/vec3.hpp"

class Frame
{
public:
    Frame(const glm::vec3& normal); // 构造函数，根据法线构造帧

    glm::vec3 world_to_screen(const glm::vec3& world_point) const; // 将世界坐标转换为屏幕坐标
    glm::vec3 screen_to_world(const glm::vec3& screen_point) const; // 将屏幕坐标转换为世界坐标

private:
    glm::vec3 m_x_axis, m_y_axis, m_z_axis;
};

#endif //FRAME_H
