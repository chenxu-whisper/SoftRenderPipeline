#include <iostream>
#include "util/profile.h"
#include "util/color.h"

Profiler::Profiler(const std::string& name) : m_name(name)
{
    m_start = std::chrono::high_resolution_clock::now(); // 记录开始时间
}
Profiler::~Profiler()
{
    auto end = std::chrono::high_resolution_clock::now(); // 记录结束时间
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count(); // 计算时间差（毫秒）
    auto duration_s = duration_ms / 1000.0f; // 计算时间差（秒）
    auto duration_min = duration_s / 60.0f; // 计算时间差（分钟）

    // 转为字符串，保留两位小数
    std::ostringstream oss;

    oss << std::fixed << std::setprecision(2) << duration_ms;
    std::string duration_ms_str = oss.str();

    oss.str(""); // 清空流
    oss << std::fixed << std::setprecision(2) << duration_s;
    std::string duration_s_str = oss.str();

    oss.str(""); // 清空流
    oss << std::fixed << std::setprecision(2) << duration_min;
    std::string duration_min_str = oss.str();

    std::cout << LOG_INFO("Profiler: ")
              << LOG_INFO(m_name) << LOG_INFO(" cost ") << LOG_INFO(duration_ms_str) << LOG_INFO("ms")
              << LOG_INFO(" (") << LOG_INFO(duration_s_str) << LOG_INFO("s | ")
              << LOG_INFO(duration_min_str) << LOG_INFO("min)") << std::endl;
}
