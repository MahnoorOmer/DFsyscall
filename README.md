# DFsyscall
Dining Philosopher


## 📌 Introduction

The **Dining Philosophers Problem** is a classic synchronization problem in operating systems. It demonstrates how improper resource sharing among concurrent processes can lead to **race conditions**, **deadlocks**, and **starvation**.

In this scenario, five philosophers sit around a circular table. Between each pair of philosophers lies a single fork. A philosopher needs both the **left** and **right** forks to eat. They alternate between **thinking** and **eating**.

The challenge arises when all philosophers try to pick up their forks simultaneously, potentially leading to:

- **Deadlock**: All philosophers hold one fork and wait forever for the other.
- **Race Conditions**: Two philosophers access the same fork concurrently.
- **Starvation**: A philosopher never gets access to both forks.

This project implements the Dining Philosophers Problem using:
- `fork()`, `wait()`, `exit()` system calls
- POSIX **named semaphores** (`sem_open`, `sem_wait`, `sem_post`, `sem_unlink`)  
All implemented in **C**.

---

## 🎯 Objective

- Simulate a concurrent system using multiple processes.
- Prevent **deadlocks** and **starvation** using **semaphores**.
- Demonstrate practical usage of system calls to solve real-world concurrency issues.

---

## 🔧 Unique Functionality in Our Solution

Each philosopher is represented by a separate process using `fork()`. The function `philosopher(int id)` defines each philosopher’s routine:

1. Think (simulated using `sleep()` and a print statement).
2. Try to pick up forks (acquire semaphores using `sem_wait()`).
3. Eat (simulated using `sleep()` and a print).
4. Put down forks (release semaphores using `sem_post()`).

### 🧠 Deadlock Prevention Strategy

We avoid circular waiting by alternating the order of fork pickup:
- **Even ID** philosophers pick up the **right fork first**, then the **left**.
- **Odd ID** philosophers pick up the **left fork first**, then the **right**.

This asymmetric order breaks the circular wait condition — one of the four Coffman conditions for deadlock.

### 🔐 Synchronization Mechanism

- Forks are modeled using **named semaphores** (via `sem_open()`).
- Semaphores are initialized to `1` (available).
- Processes share fork semaphores using unique names.

