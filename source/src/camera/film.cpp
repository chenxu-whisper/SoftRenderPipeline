#include <iostream>
#include <fstream>
#include "./camera/film.h"
#include "./util/rgb.h"


Film::Film(size_t width, size_t height)
    : m_width(width), m_height(height)
{
    m_pixels.resize(width * height);
}

size_t Film::getWidth() const { return m_width; }

size_t Film::getHeight() const { return m_height; }

Pixel Film::getPixel(size_t x, size_t y) const { return m_pixels[x + y * m_width]; }

void Film::setPixel(size_t x, size_t y, const glm::vec3 &color)
{
    m_pixels[x + y * m_width].m_color += color;
    m_pixels[x + y * m_width].m_sample_count++;
}

void Film::save(const std::filesystem::path &filepath) const
{
    // 保存图片
    // std::ios::binary 表示以二进制格式写入文件：
    // 不会对换行符进行自动转换
    // 不会对文件内容做任何文本模式下的处理
    // 确保PPM文件的二进制数据部分能够被正确写入和保存
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "无法打开文件：" << filepath.string() << std::endl;
        return;
    }

    // 写入图片头信息 P6 表示二进制格式
    file << "P6\n" << m_width << " " << m_height << "\n255\n";

    // 写入图片像素信息
    for (size_t y = 0; y < m_height; ++y)
    {
        for (size_t x = 0; x < m_width; ++x)
        {
            Pixel pixel = getPixel(x, y);
            RGB color = RGB(pixel.m_color / static_cast<float>(pixel.m_sample_count)); // 获取像素颜色
            file << color.m_r << color.m_g << color.m_b; // 写入像素值
        }
    }

    file.close();
}

Film::~Film() = default;
