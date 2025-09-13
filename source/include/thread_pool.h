#ifndef THREAD_POOL_H
#define THREAD_POOL_H


# include <vector>
# include <list>
# include <thread>
# include <mutex>

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

    void addTask(Task* task);
    Task* getTask();

private:
    bool m_alive;
    std::vector<std::thread> m_threads;  // 线程池
    std::list<Task*> m_tasks; // 任务队列
    std::mutex m_lock;  // 互斥锁
};


#endif //THREAD_POOL_H
