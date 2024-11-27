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

SystemMetrics fetch_system_metrics() {
    SystemMetrics metrics;
    metrics.cpu_usage = get_cpu_usage();
    metrics.gpu_usage = get_gpu_usage();
    metrics.memory_usage = get_memory_usage();
    metrics.disk_read_speed = get_disk_read_speed();
    metrics.disk_write_speed = get_disk_write_speed();
    metrics.network_up_speed = get_network_upload_speed();
    metrics.network_down_speed = get_network_download_speed();
    metrics.cpu_temperature = get_cpu_temperature();
    metrics.gpu_temperature = get_gpu_temperature();
    return metrics;
}
