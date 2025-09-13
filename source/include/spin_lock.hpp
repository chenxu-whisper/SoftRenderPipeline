#ifndef SPIN_LOCK_H
#define SPIN_LOCK_H


#include <atomic>
#include <thread>

class SpinLock
{
public:
    void acquire()
    {
        while (m_flag.test_and_set(std::memory_order_acquire))
            std::this_thread::yield();
    };

    void release() { m_flag.clear(std::memory_order_release); };

private:
    std::atomic_flag m_flag;
};

class SpinLockGuard
{
public:
    SpinLockGuard(SpinLock& lock):m_lock(lock) { lock.acquire(); }
    ~SpinLockGuard() { m_lock.release(); }

private:
    SpinLock& m_lock;
};


#endif //SPIN_LOCK_H