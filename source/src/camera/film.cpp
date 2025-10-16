#include <iostream>
#include <fstream>
#include "./camera/film.h"
#include "./util/rgb.h"
#include "./util/profile.h"
#include "./thread/thread_pool.h"

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

    // 用于存储像素值的缓冲区
    std::vector<uint8_t> buffer(m_width * m_height * 3); // 每个像素有3个通道（RGB）

    // 并行处理每个像素，将颜色值写入缓冲区
    thread_pool.parallelFor(m_width, m_height, [&](size_t x, size_t y)
    {
        Pixel pixel = getPixel(x, y); // 获取像素值
        RGB color = RGB(pixel.m_color / static_cast<float>(pixel.m_sample_count)); // 获取像素颜色
        // 写入像素值到缓冲区
        buffer[(x + y * m_width) * 3 + 0] = color.m_r;
        buffer[(x + y * m_width) * 3 + 1] = color.m_g;
        buffer[(x + y * m_width) * 3 + 2] = color.m_b;
    });
    thread_pool.wait(); // 等待所有线程完成任务

    // 写入图片像素信息
    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size()); // 写入缓冲区中的像素值
    file.close(); // 关闭文件流
}

void Film::clear()
{
    m_pixels.clear();
    m_pixels.resize(m_width * m_height);
}


Film::~Film() = default;
