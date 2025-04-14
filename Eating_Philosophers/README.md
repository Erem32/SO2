# Dining Philosophers Simulation with Spinlocks

## Overview

This project implements the classic Dining Philosophers problem using C++ threads and custom spinlock primitives. It demonstrates how to use spinlocks to control access to shared resources—in this case, the chopsticks—and shows how to avoid deadlocks by establishing a consistent locking order. The program simulates philosophers who alternate between thinking and eating, while properly managing concurrent resource access.

## Project Structure

- **main.cpp**  
  Contains the main function and the simulation of philosopher behavior. Each philosopher is represented by a thread that cycles between thinking and eating. Before printing to the console, the program uses a dedicated spinlock to ensure thread-safe output.

- **SpinLock.h**  
  Defines the `SpinLock` class, which utilizes `std::atomic<bool>` to create a basic spinlock mechanism. The lock method employs a busy-wait loop to achieve mutual exclusion.

- **SpinLockGuard.h**  
  Provides a RAII (Resource Acquisition Is Initialization) style wrapper around the `SpinLock`. The `SpinLockGuard` automatically acquires the lock upon construction and releases it upon destruction, ensuring exception-safe and reliable locking.

## How It Works

1. **Thinking Phase:**  
   Each philosopher first enters a thinking phase. A dedicated spinlock (printSpinLock) ensures that output to the console is synchronized across threads.

2. **Picking up Chopsticks:**  
   Each philosopher picks up two chopsticks. To avoid deadlocks, they always pick them up in a consistent order (by comparing the indices of the chopsticks). If the left chopstick’s index is lower than the right one, it is acquired first; otherwise, the right one is acquired first.

3. **Eating Phase:**  
   Once the chopsticks are acquired, the philosopher simulates eating. Console messages indicate the start and finish of the eating phase.

4. **Releasing Chopsticks:**  
   After eating, the philosopher releases both chopsticks, making them available for other philosophers.

5. **Cycle Repeat:**  
   The simulation continues indefinitely with each philosopher alternating between thinking and eating.

## Prerequisites

- **C++ Compiler:**  
  A modern C++ compiler that supports C++11 (or later). Examples include GCC, Clang, or MSVC.

- **Threading Support:**  
  The build environment must support the C++ Standard Library's threading features. Use the `-pthread` flag (or equivalent) when compiling on Linux or macOS.

## Build Instructions

To compile the project from the command line, use your preferred C++ compiler. For example, with g++:

```bash
g++ -std=c++11 main.cpp -pthread -o dining_philosophers
```

This command compiles `main.cpp` and links the necessary libraries for threading support, producing an executable named `dining_philosophers`.

## Running the Program

After building the executable, run it from the terminal:

```bash
./dining_philosophers
```

Upon execution, the program will prompt you to enter the number of philosophers. Enter a number greater than or equal to 2. The simulation will then start, continuously printing the philosophers' activities (thinking, starting to eat, finishing eating) in an infinite loop.

## Implementation Details

- **SpinLock:**  
  The `SpinLock` class uses an `std::atomic<bool>` to manage the lock state. The `lock()` method repeatedly attempts to change the lock's state from `false` to `true` until it succeeds. The `unlock()` method simply sets the state back to `false`.

- **RAII with SpinLockGuard:**  
  The `SpinLockGuard` class ensures that a spinlock is automatically released when the guard object goes out of scope, reducing the risk of deadlocks caused by forgetting to unlock.

- **Deadlock Avoidance:**  
  By ordering the lock acquisition based on the chopstick indices, the simulation avoids the scenario in which two philosophers hold one chopstick each and wait indefinitely for the other.
