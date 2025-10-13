#ifndef RGB_H
#define RGB_H

#include <glm/glm.hpp>

struct  RGB
{
    uint8_t m_r, m_g, m_b;

    // 构造函数：从RGB值创建颜色
    RGB(int r, int g, int b);

    // 构造函数：从线性空间颜色创建gamma空间颜色
    RGB(const glm::vec3& color);

    // 转换运算符：将gamma空间颜色转换为线性空间颜色
    operator glm::vec3() const;
};

#endif //RGB_H
