#include "thread_pool.h"

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
            thread_pool->m_task_count--; // 任务数量减少
        }
        else
            std::this_thread::yield();  // 没有任务时， yield 释放 CPU 时间片
    }
}

struct ParallelForTask:public Task
{
public:
    ParallelForTask(size_t x, size_t y, const std::function<void(size_t, size_t)>& lamda)
                    :m_x(x), m_y(y), m_lambda(lamda)
    {}

    void run() override { m_lambda(m_x, m_y); }

private:
    size_t m_x, m_y;
    std::function<void(size_t, size_t)> m_lambda;
};

void ThreadPool::parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)>& lambda)
{
    SpinLockGuard guard(m_lock);

    for (size_t i = 0; i < width; i++)
        for (size_t j = 0; j < height; j++)
        {
            m_tasks.push_back(new  ParallelForTask(i, j, lambda));
            m_task_count++;
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
