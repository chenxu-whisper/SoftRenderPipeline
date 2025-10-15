#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include <chrono>

#define PROFILE(name) Profiler __profiler(name)

struct Profiler
{
    std::string m_name; // 性能分析器名称
    std::chrono::high_resolution_clock::time_point m_start; // 开始时间

    Profiler(const std::string& name);
    ~Profiler();
};

#endif //PROFILE_H
