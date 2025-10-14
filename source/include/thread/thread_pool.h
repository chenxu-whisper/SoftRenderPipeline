#ifndef THREAD_POOL_H
#define THREAD_POOL_H


# include <vector>
# include <list>
# include <thread>
# include <functional>
#include "spin_lock.hpp"

// 任务基类
class Task
{
public:
    virtual void run() = 0;
    virtual ~Task() = default;
};

// 线程池类
class ThreadPool
{
public:
    ThreadPool(size_t threads_num = 0);
    ~ThreadPool();

    static void workerThread(ThreadPool* thread_pool);

    void parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)>& func);
    void wait() const;

    void addTask(Task* task);
    Task* getTask();

private:
    std::atomic<int> m_alive;
    std::vector<std::thread> m_threads;  // 线程池
    std::atomic<int> m_task_count;
    std::list<Task*> m_tasks; // 任务队列
    SpinLock m_lock; // 自旋锁
};

extern ThreadPool thread_pool;

#endif //THREAD_POOL_H
