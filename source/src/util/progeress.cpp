#include <iostream>
#include "./util/progress.h"

Progress::Progress(size_t total, int step)
    :m_total(total), m_step(step), m_current(0), m_current_percent(0), m_last_percent(0)
{
    std::cout << "0%" << std::endl;
}

void Progress::update(size_t count)
{
    SpinLockGuard guard(m_spin_lock);

    m_current += count;
    m_current_percent = 100 * static_cast<float>(m_current) / static_cast<float>(m_total);
    if ((m_current_percent - m_last_percent >= m_step) || (m_current_percent == 100))
    {
        m_last_percent = m_current_percent;
        std::cout << m_current_percent << "%" << std::endl;
    }
}

