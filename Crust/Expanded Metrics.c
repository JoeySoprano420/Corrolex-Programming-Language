#include <stdio.h>
#include <sys/statvfs.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Function to get disk usage
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

// Function to get network traffic
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

// Function to simulate GPU monitoring (placeholder for NVIDIA GPU integration)
float get_gpu_usage() {
    // Add calls to nvidia-smi or similar libraries for real monitoring
    return 50.0; // Placeholder for GPU usage in percentage
}

// Updated monitor function
void monitor_system_extended() {
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
}
