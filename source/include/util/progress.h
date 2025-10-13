#ifndef PROGRESS_H
#define PROGRESS_H

#include "./thread/spin_lock.hpp"

class Progress
{
public:
    Progress(size_t total, int step = 1);
    void update(size_t count);

private:
    size_t m_total; // 总任务数
    size_t m_current; // 当前完成任务数
    int m_current_percent; // 当前进度百分比
    int m_last_percent; // 上一次打印的进度百分比
    int m_step; // 进度打印步长
    SpinLock m_spin_lock;
};

#endif //PROGRESS_H
