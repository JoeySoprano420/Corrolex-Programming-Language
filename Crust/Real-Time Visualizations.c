#include <ncurses.h>
#include <unistd.h>

// ncurses-based system monitor
void visualize_monitoring_ncurses() {
    initscr();
    noecho();
    curs_set(FALSE);

    while (1) {
        float cpu_usage = get_cpu_usage();
        long total_mem, free_mem;
        get_memory_usage(&total_mem, &free_mem);

        clear();
        mvprintw(0, 0, "System Metrics:");
        mvprintw(2, 0, "CPU Usage: %.2f%%", cpu_usage);
        mvprintw(3, 0, "Memory Usage: %ld MB / %ld MB", total_mem - free_mem, total_mem);
        
        refresh();
        sleep(1);
    }

    endwin();
}
