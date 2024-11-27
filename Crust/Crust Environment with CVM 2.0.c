#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h> // Terminal GUI (Violet Aura theme with animations)
#include <pthread.h>
#include <unistd.h>

// System Metrics Structure
typedef struct {
    float cpu_usage;
    float gpu_usage;
    long memory_usage;
    long disk_read_speed;
    long disk_write_speed;
    float network_up_speed;
    float network_down_speed;
    float cpu_temperature;
    float gpu_temperature;
} SystemMetrics;

// Function to Fetch Metrics
SystemMetrics fetch_system_metrics() {
    SystemMetrics metrics = {70.5, 55.3, 8096, 120, 100, 50.5, 30.2, 60.0, 45.0};
    return metrics; // Simulated metrics
}

// Violet Aura GUI Functions
void draw_holographic_frame() {
    for (int i = 0; i < 80; i++) {
        mvprintw(0, i, "=");
        mvprintw(24, i, "=");
    }
    for (int i = 1; i < 24; i++) {
        mvprintw(i, 0, "|");
        mvprintw(i, 79, "|");
    }
    mvprintw(0, 35, " VIOLET AURA UNIVERSE ");
}

void draw_dynamic_background() {
    for (int y = 1; y < 24; y++) {
        for (int x = 1; x < 79; x++) {
            mvprintw(y, x, ".");
        }
        usleep(5000);
        refresh();
    }
}

void show_metrics(SystemMetrics metrics) {
    mvprintw(2, 2, "System Metrics (Violet Aura Dashboard)");
    mvprintw(4, 2, "CPU Usage: %.2f%%", metrics.cpu_usage);
    mvprintw(5, 2, "GPU Usage: %.2f%%", metrics.gpu_usage);
    mvprintw(6, 2, "Memory Usage: %ld MB", metrics.memory_usage);
    mvprintw(7, 2, "Disk Read Speed: %ld MB/s", metrics.disk_read_speed);
    mvprintw(8, 2, "Disk Write Speed: %ld MB/s", metrics.disk_write_speed);
    mvprintw(9, 2, "Network Upload Speed: %.2f Mbps", metrics.network_up_speed);
    mvprintw(10, 2, "Network Download Speed: %.2f Mbps", metrics.network_down_speed);
    mvprintw(11, 2, "CPU Temperature: %.2f°C", metrics.cpu_temperature);
    mvprintw(12, 2, "GPU Temperature: %.2f°C", metrics.gpu_temperature);
}

// Corrolex Script Execution
void execute_corrolex_script(const char *script) {
    mvprintw(14, 2, "Executing Corrolex Script...");
    refresh();
    sleep(2);
    mvprintw(15, 2, "Script Output: 'Hello from Corrolex!'");
}

// Multi-threaded System Monitoring
void *monitor_system(void *arg) {
    while (1) {
        SystemMetrics metrics = fetch_system_metrics();
        show_metrics(metrics);
        refresh();
        sleep(1); // Update every second
    }
    return NULL;
}

int main() {
    // Initialize ncurses
    initscr();
    noecho();
    curs_set(FALSE);

    // Draw GUI and Initialize Threads
    draw_holographic_frame();
    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, monitor_system, NULL);

    // Simulate Dynamic Background
    draw_dynamic_background();

    // Execute Corrolex Script
    execute_corrolex_script("print('Hello World!')");

    // Wait for User Input to Exit
    mvprintw(23, 2, "Press any key to exit...");
    getch();

    // Cleanup
    endwin();
    pthread_cancel(monitor_thread);
    pthread_join(monitor_thread, NULL);

    return 0;
}
