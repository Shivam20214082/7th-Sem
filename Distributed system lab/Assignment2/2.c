// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>

#define SHM_NAME "/my_shm"  // Shared memory object name
#define SEM_NAME "/my_sem"  // Semaphore name
#define COUNTER_SIZE sizeof(int)  // Size of the counter

int main() {
    // File descriptor for shared memory
    int shm_fd;
    // Pointer to shared memory
    int *counter;
    // Semaphore
    sem_t *sem;
    
    // Create and open shared memory object
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    // Set the size of the shared memory object
    if (ftruncate(shm_fd, COUNTER_SIZE) < 0) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    
    // Map shared memory object into address space
    counter = mmap(NULL, COUNTER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (counter == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
    // Create and initialize semaphore
    sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Initialize counter to 0
    *counter = 0;

    // Increment counter in a loop
    for (int i = 0; i < 10; i++) {
        // Wait (lock) the semaphore
        sem_wait(sem);
        
        // Critical section: increment the counter
        (*counter)++;
        printf("Counter: %d\n", *counter);
        
        // Signal (unlock) the semaphore
        sem_post(sem);
        
        // Sleep for a bit to simulate work
        sleep(1);
    }

    // Clean up
    if (munmap(counter, COUNTER_SIZE) < 0) {
        perror("munmap");
    }
    
    if (close(shm_fd) < 0) {
        perror("close");
    }

    if (shm_unlink(SHM_NAME) < 0) {
        perror("shm_unlink");
    }
    
    if (sem_close(sem) < 0) {
        perror("sem_close");
    }

    if (sem_unlink(SEM_NAME) < 0) {
        perror("sem_unlink");
    }

    return 0;
}
