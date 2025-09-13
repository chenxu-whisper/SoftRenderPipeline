/*
    * 在计算机图形学中，二维图像通常以一维数组的形式存储在内存中。表达式 x + y * m_width 的含义是：
    * 图像的宽度（每行的像素数量）
    * m_pixels 是一个一维数组，按行主序（row-major order）存储所有像素的颜色值
    * y * m_width 计算当前行之前所有行的总像素数
    * 加上 x（当前行中的列索引）就得到了该像素在一维数组中的精确位置
    * 这种存储和访问方式在图形编程中非常常见，它能够高效地将二维坐标转换为一维内存地址。
    *
    *
    * PPM（Portable Pixmap Format）是一种简单的便携式像素图格式，属于Netpbm图像格式家族。它的主要特点包括：
    * 格式简单：PPM文件使用ASCII或二进制格式存储图像数据，结构直观，易于程序读写
    * 未压缩：不进行压缩，保留原始图像质量，但文件体积较大
    * 支持彩色：可以存储RGB三色通道的图像数据
    * PPM文件的基本结构包括：
    *   魔数（Magic Number）：P3（ASCII格式）或P6（二进制格式）
    *   图像宽度和高度：分别表示图像的水平像素数和垂直像素数
    *   最大颜色值（通常为255）：表示每个颜色通道的最大值，用于归一化颜色值
    *   像素数据：按行主序存储的RGB值，每个像素占用3个字节（或24位）
 */

#ifndef FILM_H
#define FILM_H

#include <filesystem>
#include <vector>
#include <glm/glm.hpp>

class Film
{
public:
    Film(size_t width, size_t height);
    ~Film();

    size_t getWidth() const;
    size_t getHeight() const;
    glm::vec3 getPixel(size_t x, size_t y) const; // 获取像素值
    void setPixel(size_t x, size_t y, const glm::vec3& color); // 像素值范围[0, 1]
    void save(const std::filesystem::path&  filepath) const; // 保存为PPM格式

private:
    size_t m_width;
    size_t m_height;
    std::vector<glm::vec3> m_pixels;
};


#endif //FILM_H
