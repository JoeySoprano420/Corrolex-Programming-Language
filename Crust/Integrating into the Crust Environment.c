#include <pthread.h>
#include <stdio.h>

// Function declarations
void monitor_system();
void crust_runtime();

// Main function
int main() {
    printf("Initializing Crust Environment...\n");

    // Start monitoring system health in a separate thread
    pthread_t monitor_thread;
    if (pthread_create(&monitor_thread, NULL, (void *(*)(void *))monitor_system, NULL) != 0) {
        perror("Failed to create monitoring thread");
        return 1;
    }

    // Simulate Crust runtime functionality
    crust_runtime();

    // Join monitoring thread (program will run indefinitely for demo purposes)
    pthread_join(monitor_thread, NULL);
    return 0;
}

// Simulate Crust runtime (placeholder)
void crust_runtime() {
    printf("Running Crust Runtime...\n");
    for (int i = 0; i < 10; i++) {
        printf("Executing Task #%d...\n", i + 1);
        sleep(2); // Simulate task delay
    }
}
