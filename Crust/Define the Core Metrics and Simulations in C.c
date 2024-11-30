#include <emscripten.h>
#include <stdlib.h>
#include <time.h>

// ML-Plus Simulation Variables
typedef struct {
    double cpu_usage;
    double memory_usage;
    double gpu_usage;
    double anomaly_probability;
} SystemMetrics;

// Generate Random Metrics for Simulations
EMSCRIPTEN_KEEPALIVE
SystemMetrics simulate_system_metrics() {
    SystemMetrics metrics;
    srand(time(NULL));

    metrics.cpu_usage = 50.0 + (rand() % 50); // Simulated CPU usage (50-100%)
    metrics.memory_usage = 40.0 + (rand() % 60); // Simulated Memory usage (40-100%)
    metrics.gpu_usage = 30.0 + (rand() % 70); // Simulated GPU usage (30-100%)
    metrics.anomaly_probability = (rand() % 100) / 100.0; // Probability (0.0 to 1.0)

    return metrics;
}

// Anomaly Detection
EMSCRIPTEN_KEEPALIVE
int detect_anomalies(SystemMetrics metrics) {
    if (metrics.cpu_usage > 90.0 || metrics.memory_usage > 90.0 || metrics.anomaly_probability > 0.8) {
        return 1; // Anomaly detected
    }
    return 0; // No anomaly
}

// Predictive Optimization (Mock Simulation)
EMSCRIPTEN_KEEPALIVE
SystemMetrics optimize_resources(SystemMetrics metrics) {
    // Simple logic: reduce usage if metrics are high
    if (metrics.cpu_usage > 90.0) metrics.cpu_usage -= 10.0;
   
