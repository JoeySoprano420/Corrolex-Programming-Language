#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/sysinfo.h> // For system metrics
#include <unistd.h>      // For sleep()

// Function to get CPU usage percentage
float get_cpu_usage() {
    static long prev_idle = 0, prev_total = 0;
    long idle, total, diff_idle, diff_total;
    float cpu_usage;

    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("Error opening /proc/stat");
        return -1.0;
    }

    char buffer[128];
    fgets(buffer, sizeof(buffer), fp); // Read first line
    fclose(fp);

    long user, nice, system, idle_now, iowait, irq, softirq, steal;
    sscanf(buffer, "cpu %ld %ld %ld %ld %ld %ld %ld %ld",
           &user, &nice, &system, &idle_now, &iowait, &irq, &softirq, &steal);

    idle = idle_now + iowait;
    total = user + nice + system + idle + irq + softirq + steal;

    diff_idle = idle - prev_idle;
    diff_total = total - prev_total;

    prev_idle = idle;
    prev_total = total;

    cpu_usage = 100.0 * (1.0 - (float)diff_idle / diff_total);
    return cpu_usage;
}

// Function to get total and free memory
void get_memory_usage(long *total, long *free) {
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        perror("sysinfo error");
        *total = *free = 0;
        return;
    }
    *total = info.totalram / 1024 / 1024; // Convert to MB
    *free = info.freeram / 1024 / 1024;   // Convert to MB
}

// System monitoring function
void monitor_system() {
    while (1) {
        float cpu_usage = get_cpu_usage();
        long total_mem, free_mem;
        get_memory_usage(&total_mem, &free_mem);

        // Timestamp for logs
        time_t now = time(NULL);
        char *timestamp = ctime(&now);
        timestamp[strcspn(timestamp, "\n")] = 0; // Remove newline character

        // Print system metrics
        printf("\n[%s] System Metrics:\n", timestamp);
        printf("CPU Usage: %.2f%%\n", cpu_usage);
        printf("Total Memory: %ld MB\n", total_mem);
        printf("Free Memory: %ld MB\n", free_mem);
        printf("Used Memory: %ld MB\n", total_mem - free_mem);

        // Add your future monitoring expansions here...

        sleep(5); // Monitor every 5 seconds
    }
}
