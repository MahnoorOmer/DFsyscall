#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define THINKING_TIME 1
#define EATING_TIME 2
#define CYCLES 3 // Number of times each philosopher eats

// Semaphores representing forks
sem_t forks[NUM_PHILOSOPHERS];

// Mutex for synchronized logging
pthread_mutex_t log_mutex;

// Utility function to print logs in a thread-safe way
void log_action(int id, const char *action) {
    pthread_mutex_lock(&log_mutex);
    printf("Philosopher %d %s\n", id, action);
    fflush(stdout);
    pthread_mutex_unlock(&log_mutex);
}

// Philosopher routine
void* philosopher(void* arg) {
    int id = *(int*)arg;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    for (int i = 0; i < CYCLES; ++i) {
        // Thinking
        log_action(id, "is thinking...");
        sleep(THINKING_TIME);

        // Deadlock prevention strategy:
        // Even philosophers pick right fork first, odd pick left fork first
        if (id % 2 == 0) {
            sem_wait(&forks[right]);
            log_action(id, "picked up right fork");
            sem_wait(&forks[left]);
            log_action(id, "picked up left fork");
        } else {
            sem_wait(&forks[left]);
            log_action(id, "picked up left fork");
            sem_wait(&forks[right]);
            log_action(id, "picked up right fork");
        }

        // Eating
        log_action(id, "is eating...");
        sleep(EATING_TIME);

        // Put down forks
        sem_post(&forks[left]);
        sem_post(&forks[right]);
        log_action(id, "put down both forks and is done eating");
    }

    log_action(id, "has finished all cycles and left the table");
    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize mutex
    pthread_mutex_init(&log_mutex, NULL);

    // Initialize semaphores for forks
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        sem_init(&forks[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        ids[i] = i;
        if (pthread_create(&philosophers[i], NULL, philosopher, &ids[i]) != 0) {
            perror("Failed to create philosopher thread");
            exit(1);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores and mutex
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        sem_destroy(&forks[i]);
    }
    pthread_mutex_destroy(&log_mutex);

    printf("\nAll philosophers have finished their meals. Simulation complete.\n");
    return 0;
}
