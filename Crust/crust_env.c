#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <ncurses.h>
#include <pthread.h>

// ------------------------------ METRICS FUNCTIONS ----------------------------

// CPU usage simulation (can be replaced with real system calls)
float get_cpu_usage() {
    // Placeholder for real CPU usage
    return rand() % 100;
}

// Memory usage simulation (can use sysinfo for real metrics)
void get_memory_usage(long *total, long *free) {
    *total = 8192;  // Simulated total memory in MB
    *free = rand() % 4096;  // Simulated free memory in MB
}

// Disk usage
void get_disk_usage(const char *path, long *total, long *used) {
    struct statvfs stats;
    if (statvfs(path, &stats) != 0) {
        perror("Failed to get disk usage");
        *total = *used = 0;
        return;
    }
    *total = (stats.f_blocks * stats.f_frsize) / (1024 * 1024); // Convert to MB
    *used = ((stats.f_blocks - stats.f_bfree) * stats.f_frsize) / (1024 * 1024); // Convert to MB
}

// Network traffic monitoring
void get_network_traffic(long *rx_bytes, long *tx_bytes) {
    FILE *fp = fopen("/proc/net/dev", "r");
    if (!fp) {
        perror("Failed to read /proc/net/dev");
        *rx_bytes = *tx_bytes = 0;
        return;
    }

    char buffer[256];
    *rx_bytes = *tx_bytes = 0;

    // Skip the first two lines (header)
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    while (fgets(buffer, sizeof(buffer), fp)) {
        char iface[32];
        long rx, tx;
        sscanf(buffer, "%s %ld %*d %*d %*d %*d %*d %*d %*d %ld", iface, &rx, &tx);
        *rx_bytes += rx;
        *tx_bytes += tx;
    }

    fclose(fp);
}

// GPU usage simulation
float get_gpu_usage() {
    // Placeholder for GPU usage (replace with real GPU metrics)
    return 50.0 + (rand() % 50);
}

// --------------------------- SYSTEM MONITORING -------------------------------

// Monitor system and print metrics in real time
void *monitor_system_extended(void *arg) {
    while (1) {
        // Basic metrics
        float cpu_usage = get_cpu_usage();
        long total_mem, free_mem;
        get_memory_usage(&total_mem, &free_mem);

        // Extended metrics
        long total_disk, used_disk;
        get_disk_usage("/", &total_disk, &used_disk);

        long rx_bytes, tx_bytes;
        get_network_traffic(&rx_bytes, &tx_bytes);

        float gpu_usage = get_gpu_usage();

        // Print all metrics
        printf("\nSystem Metrics:\n");
        printf("CPU Usage: %.2f%%\n", cpu_usage);
        printf("Memory Usage: %ld MB / %ld MB\n", total_mem - free_mem, total_mem);
        printf("Disk Usage: %ld MB / %ld MB\n", used_disk, total_disk);
        printf("Network Traffic: RX: %ld bytes, TX: %ld bytes\n", rx_bytes, tx_bytes);
        printf("GPU Usage: %.2f%%\n", gpu_usage);

        sleep(5); // Monitor every 5 seconds
    }
    return NULL;
}

// --------------------------- CORROLEX INTERPRETER ----------------------------

// Execute a Corrolex script
void execute_corrolex_script(const char *script) {
    printf("Executing Corrolex Script:\n%s\n", script);
    
    // Parse and execute Corrolex commands
    if (strstr(script, "PRINT")) {
        printf("Corrolex Output: %s\n", script + 6); // Extract after PRINT command
    } else if (strstr(script, "MONITOR")) {
        monitor_system_extended(NULL); // Call system monitor
    } else {
        printf("Unknown Corrolex command.\n");
    }
}

// Corrolex script runtime
void crust_runtime_with_corrolex() {
    char script[256];
    printf("Enter a Corrolex script (type 'exit' to quit):\n");
    while (1) {
        printf(">>> ");
        fgets(script, sizeof(script), stdin);
        script[strcspn(script, "\n")] = 0; // Remove newline character

        if (strcmp(script, "exit") == 0) break;

        execute_corrolex_script(script);
    }
}

// ----------------------------- VISUALIZATION ---------------------------------

// Visualize system metrics in ncurses
void *visualize_monitoring_ncurses(void *arg) {
    initscr();
    noecho();
    curs_set(FALSE);

    while (1) {
        float cpu_usage = get_cpu_usage();
        long total_mem, free_mem;
        get_memory_usage(&total_mem, &free_mem);
        long total_disk, used_disk;
        get_disk_usage("/", &total_disk, &used_disk);

        clear();
        mvprintw(0, 0, "System Metrics:");
        mvprintw(2, 0, "CPU Usage: %.2f%%", cpu_usage);
        mvprintw(3, 0, "Memory Usage: %ld MB / %ld MB", total_mem - free_mem, total_mem);
        mvprintw(4, 0, "Disk Usage: %ld MB / %ld MB", used_disk, total_disk);
        
        refresh();
        sleep(1);
    }

    endwin();
    return NULL;
}

// ------------------------------- MAIN PROGRAM --------------------------------

int main() {
    pthread_t monitor_thread, visualize_thread;

    printf("Starting Crust Environment...\n");

    // Launch system monitoring in the background
    pthread_create(&monitor_thread, NULL, monitor_system_extended, NULL);

    // Launch ncurses visualization in the background
    pthread_create(&visualize_thread, NULL, visualize_monitoring_ncurses, NULL);

    // Start Corrolex runtime for user interaction
    crust_runtime_with_corrolex();

    // Cleanup
    pthread_cancel(monitor_thread);
    pthread_cancel(visualize_thread);
    pthread_join(monitor_thread, NULL);
    pthread_join(visualize_thread, NULL);

    printf("Exiting Crust Environment...\n");
    return 0;
}
