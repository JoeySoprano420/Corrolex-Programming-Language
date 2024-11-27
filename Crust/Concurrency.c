void execute_concurrent_tasks(CorrolexInstruction *tasks, size_t task_count) {
    #pragma omp parallel for
    for (size_t i = 0; i < task_count; i++) {
        execute_task(&tasks[i]);
    }
}

void execute_parallel(CorrolexTask *tasks, size_t task_count) {
    #pragma omp parallel for
    for (size_t i = 0; i < task_count; i++) {
        tasks[i].execute();
    }
}
