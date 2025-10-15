#include "./thread/thread_pool.h"

#include <cmath>

#include "./util/profile.h"

ThreadPool thread_pool;

ThreadPool::ThreadPool(size_t threads_num)
{
    m_alive = 1;
    m_task_count = 0;
    // 线程数为0时，获取硬件线程数
    if (threads_num == 0)
        threads_num = std::thread::hardware_concurrency();

    // 创建线程
    for (size_t i = 0; i < threads_num; i++)
        m_threads.push_back(std::thread(workerThread, this));
}

void ThreadPool::workerThread(ThreadPool *thread_pool)
{
    while (thread_pool->m_alive == 1)
    {
        // 从任务队列中获取任务
        Task* task = thread_pool->getTask();
        if (task != nullptr)
        {
            task->run(); // 执行任务
            delete task; // 释放任务内存
            thread_pool->m_task_count--; // 任务数量减少
        }
        else
            std::this_thread::yield();  // 没有任务时， yield 释放 CPU 时间片
    }
}

struct ParallelForTask:public Task
{
public:
    ParallelForTask(size_t x, size_t y, size_t chunk_width, size_t chunk_height, const std::function<void(size_t, size_t)>& lamda)
                    :m_x(x), m_y(y), m_chunk_width(chunk_width), m_chunk_height(chunk_height), m_lambda(lamda)
    {}

    void run() override
    {
        for (size_t i = 0; i < m_chunk_width; i++)
            for (size_t j = 0; j < m_chunk_height; j++)
                m_lambda(m_x + i, m_y + j);
    }

private:
    size_t m_x, m_y, m_chunk_width, m_chunk_height;
    std::function<void(size_t, size_t)> m_lambda;
};

void ThreadPool::parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)>& lambda)
{
    PROFILE("ParallelFor ");

    SpinLockGuard guard(m_lock);

    // 区域划分渲染，每个线程处理的宽度和高度
    int chunk_width = static_cast<int>(std::ceil(static_cast<float>(width) / std::sqrt(16.0f) / static_cast<float>(m_threads.size())));
    int chunk_height = static_cast<int>(std::ceil(static_cast<float>(height) / std::sqrt(16.0f) / static_cast<float>(m_threads.size())));
    // 处理任务
    for (size_t x = 0; x < width; x+=chunk_width)
    {
        for (size_t y = 0; y < height; y+=chunk_height)
        {
            // 处理边界情况
            if (x + chunk_width > width)
                chunk_width = width - x; // 处理最后一列时，宽度可能会小于 chunk_width
            if (y + chunk_height > height)
                chunk_height = height - y; // 处理最后一行时，高度可能会小于 chunk_height

            m_tasks.push_back(new  ParallelForTask(x, y, chunk_width, chunk_height, lambda)); // 添加任务
            m_task_count++; // 任务数量增加
        }
    }
}

void ThreadPool::wait() const
{
    // 等待任务队列清空
    while (m_task_count > 0)
        std::this_thread::yield();
}

void ThreadPool::addTask(Task *task)
{
    // 加锁
    SpinLockGuard guard(m_lock);
    // 任务数量增加
    m_task_count++;
    // 添加任务
    m_tasks.push_back(task);
}

Task * ThreadPool::getTask()
{
    // 加锁
    SpinLockGuard guard(m_lock);

    if (m_tasks.empty())
        return nullptr;
    // 获取任务
    Task* task = m_tasks.front();
    m_tasks.pop_front();

    return task;
}

ThreadPool::~ThreadPool()
{
    // 等待任务队列清空
    wait();
    // 通知所有线程退出
    m_alive = 0;

    // 等待所有线程执行完毕
    for (auto& thread : m_threads)
        thread.join();

    // 清空线程池
    m_threads.clear();
}
