#include "frame.h"
#include "glm/glm.hpp"

Frame::Frame(const glm::vec3 &normal)
{
    m_y_axis = normal;
    glm::vec3 up = abs(normal.y) < 0.999f ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(0.0f, 0.0f, 1.0f);
    m_x_axis = glm::normalize(glm::cross(up, normal));
    m_z_axis = glm::cross(m_x_axis, m_y_axis);
}



glm::vec3 Frame::world_to_screen(const glm::vec3 &world_point) const
{

    /*为什么用点积而不是叉积？
      * 因为点积的结果是一个标量，而叉积的结果是一个向量
      * 我们需要的是一个标量，用来表示点在帧的哪个方向上
      * 所以用点积可以直接得到这个标量
      * 然后用这个标量构造一个向量，就可以表示点在帧的哪个方向上
      * 最后用这个向量归一化，就可以得到点在帧的单位向量
      * 这个单位向量就表示了点在帧的哪个方向上
    */
    return glm::normalize(glm::vec3(glm::dot(world_point, m_x_axis), glm::dot(world_point, m_y_axis), glm::dot(world_point, m_z_axis)));
}

glm::vec3 Frame::screen_to_world(const glm::vec3 &screen_point) const
{
    /*为什么相加得到世界坐标？
     * 因为 screen_point 是一个单位向量，它的每个分量都表示了点在帧的哪个方向上
     * 所以我们可以用 screen_point 的每个分量乘以帧的坐标轴，得到点在世界坐标系中的坐标
     * 最后用归一化，就可以得到点在世界坐标系中的单位向量
     */
    return glm::normalize(screen_point.x * m_x_axis + screen_point.y * m_y_axis + screen_point.z * m_z_axis);

}

