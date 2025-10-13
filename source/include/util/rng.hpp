#ifndef RNG_H
#define RNG_H

#include <random>

class RNG
{
public:
    RNG(size_t seed) { set_seed(seed); }
    RNG() : RNG(0) { };

    void set_seed(size_t seed) { m_gen.seed(seed); }; // 设置随机数种子
    float uniform() { return m_dis(m_gen); }; // 生成均匀分布的随机数

private:
    std::mt19937_64 m_gen; // 随机数生成器
    std::uniform_real_distribution<float> m_dis{ 0, 1}; // 均匀分布
};

#endif //RNG_H
