#include "thread_pool.h"

ThreadPool::ThreadPool(size_t threads_num)
{
    m_alive = true;
    // 线程数为0时，获取硬件线程数
    if (threads_num == 0)
        threads_num = std::thread::hardware_concurrency();

    // 创建线程
    for (size_t i = 0; i < threads_num; i++)
        m_threads.push_back(std::thread(workerThread, this));
}

void ThreadPool::workerThread(ThreadPool *thread_pool)
{
    while (thread_pool->m_alive)
    {
        // 从任务队列中获取任务
        Task* task = thread_pool->getTask();
        if (task != nullptr)
            task->run(); // 执行任务
        else
            std::this_thread::yield();  // 没有任务时， yield 释放 CPU 时间片
    }
}

void ThreadPool::addTask(Task *task)
{
    // 加锁
    std::lock_guard<std::mutex> lock(m_lock);
    // 添加任务
    m_tasks.push_back(task);
}

Task * ThreadPool::getTask()
{
    // 加锁
    std::lock_guard<std::mutex> lock(m_lock);

    if (m_tasks.empty())
        return nullptr;
    // 获取任务
    Task* task = m_tasks.front();
    m_tasks.pop_front();

    return task;
}

ThreadPool::~ThreadPool()
{
    while (!m_tasks.empty()) { }
    m_alive = false;

    // 等待所有线程执行完毕
    for (auto& thread : m_threads)
        thread.join();

    // 清空线程池
    m_threads.clear();
}
