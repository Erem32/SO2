#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// Include our spinlock classes
#include "SpinLock.h"
#include "SpinLockGuard.h"

using namespace std;

// SpinLock for printing
static SpinLock printSpinLock;

// Global data
static vector<SpinLock> chopsticks; // One SpinLock per philosopher
static int number_of_philosophers = 0;

// Simulates one philosopher's behavior
void philosopher(int id)
{
    while (true)
    {
        // 1. Thinking
        {
            // Lock the print spinlock before writing to std::cout
            SpinLockGuard lock(printSpinLock);
            cout << "Philosopher " << id << " is thinking.\n";
        }
        this_thread::sleep_for(chrono::milliseconds(500));

        // Identify chopsticks on the left and right
        int left = id;
        int right = (id + 1) % number_of_philosophers;

        // 2. Picking up chopsticks in a consistent order
        if (left < right)
        {
            chopsticks[left].lock();
            chopsticks[right].lock();
        }
        else
        {
            chopsticks[right].lock();
            chopsticks[left].lock();
        }

        // 3. Eating
        {
            SpinLockGuard lock(printSpinLock);
            cout << "Philosopher " << id << " starts eating.\n";
        }
        this_thread::sleep_for(chrono::milliseconds(500));
        {
            SpinLockGuard lock(printSpinLock);
            cout << "Philosopher " << id << " finishes eating.\n";
        }

        // 4. Putting down chopsticks
        chopsticks[left].unlock();
        chopsticks[right].unlock();
    }
}

int main()
{
    cout << "Enter the number of philosophers: ";
    cin >> number_of_philosophers;

    if (number_of_philosophers < 2)
    {
        cerr << "You must have at least 2 philosophers.\n";
        return 1;
    }

    // Construct exactly 'number_of_philosophers' spinlocks
    // Safe couse SpinLock is movable
    chopsticks.reserve(number_of_philosophers);
    for (int i = 0; i < number_of_philosophers; i++)
    {
        chopsticks.emplace_back(); // default-construct each SpinLock
    }

    // Create and launch threads
    vector<thread> philosophers;
    philosophers.reserve(number_of_philosophers);
    for (int i = 0; i < number_of_philosophers; ++i)
    {
        philosophers.emplace_back(philosopher, i);
    }

    // Ensuring that all threads stay running ( main doesn't exit before all threads finish running)
    for (auto &ph : philosophers)
    {
        ph.join();
    }

    return 0;
}
