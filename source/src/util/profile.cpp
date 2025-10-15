#include <iostream>
#include "util/profile.h"

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

    std::cout << "Profiler: "
              << m_name << " cost " << duration_ms << "ms"
              << " (" << std::fixed << std::setprecision(2) << duration_s << "s | "
              << std::fixed << std::setprecision(2) << duration_min << "min)" << std::endl;
}
