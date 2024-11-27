void scale_memory_pools(size_t active_tasks) {
    if (active_tasks > HIGH_LOAD_THRESHOLD) {
        increase_memory_pool_size();
    } else if (active_tasks < LOW_LOAD_THRESHOLD) {
        decrease_memory_pool_size();
    }
}
