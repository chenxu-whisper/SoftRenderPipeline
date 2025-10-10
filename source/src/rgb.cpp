#include "rgb.h"

RGB::RGB(int r, int g, int b) : m_r(r), m_g(g), m_b(b) { }

/* gamma空间：
 * gamma空间是一种用于表示颜色的空间，它将颜色的每个通道的值映射到0到1之间的范围。
 * 与线性空间不同，gamma空间对颜色的低值进行了压缩，而对高值进行了扩展。
 * 这使得gamma空间在显示设备上显示颜色时能够更准确地表示颜色的细节。
*/

//  线性空间转换为gamma空间
RGB::RGB(const glm::vec3& color)
{
    m_r = glm::clamp<uint8_t>(glm::pow(color.r, 1.0f / 2.2f) * 255.0f, 0, 255);
    m_g = glm::clamp<uint8_t>(glm::pow(color.g, 1.0f / 2.2f) * 255.0f, 0, 255);
    m_b = glm::clamp<uint8_t>(glm::pow(color.b, 1.0f / 2.2f) * 255.0f, 0, 255);
}

// gamma空间转换为线性空间
RGB:: operator glm::vec3() const
{
    return glm::pow(glm::vec3(m_r, m_g, m_b) / 255.0f, glm::vec3(2.2));
}
