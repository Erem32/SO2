#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <atomic>

// Attempts to acquire the lock in a loop ("spinning") until it succeeds.
class SpinLock
{
public:
    SpinLock() : locked(false) {}

    // Explicit empty move constructor/assignment
    SpinLock(SpinLock &&) noexcept {}
    SpinLock &operator=(SpinLock &&) noexcept { return *this; }

    // Delete copy constructor and copy assignment
    SpinLock(const SpinLock &) = delete;
    SpinLock &operator=(const SpinLock &) = delete;

    // Lock by spinning until set locked from false to true
    void lock()
    {
        bool expected = false;
        while (!locked.compare_exchange_weak(expected, true,
                                             std::memory_order_acquire, std::memory_order_relaxed))
        {
            expected = false;
        }
    }

    // Unlock sets 'locked' back to false
    void unlock()
    {
        locked.store(false, std::memory_order_release);
    }

private:
    std::atomic<bool> locked;
};

#endif // SPINLOCK_H
