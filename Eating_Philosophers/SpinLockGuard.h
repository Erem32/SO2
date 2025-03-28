#ifndef SPINLOCK_GUARD_H
#define SPINLOCK_GUARD_H

#include "SpinLock.h"

// When this object is created, it locks the given SpinLock.
// When the object is destroyed, it unlocks the SpinLock.

class SpinLockGuard
{
public:
    explicit SpinLockGuard(SpinLock &s) : spin(s)
    {
        spin.lock();
    }
    ~SpinLockGuard()
    {
        spin.unlock();
    }

    // No copying
    SpinLockGuard(const SpinLockGuard &) = delete;
    SpinLockGuard &operator=(const SpinLockGuard &) = delete;

private:
    SpinLock &spin;
};

#endif // SPINLOCK_GUARD_H
