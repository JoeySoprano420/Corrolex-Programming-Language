let Module; // Load WebAssembly Module

// Wait for the Wasm Module to initialize
document.addEventListener('DOMContentLoaded', () => {
    Module = Module || {};
});

// Fetch System Metrics
function simulateMetrics() {
    const simulate = Module.cwrap('simulate_system_metrics', 'number', []);
    const metricsPtr = simulate();
    const metrics = new Float64Array(Module.HEAPF64.buffer, metricsPtr, 4);

    document.getElementById('cpu').textContent = metrics[0].toFixed(2);
    document.getElementById('memory').textContent = metrics[1].toFixed(2);
    document.getElementById('gpu').textContent = metrics[2].toFixed(2);
    document.getElementById('anomaly').textContent = metrics[3].toFixed(2);
}

// Optimize Resources
function optimizeResources() {
    const optimize = Module.cwrap('optimize_resources', 'number', ['number']);
    const metricsPtr = simulateMetrics(); // Simulate first
    const optimizedPtr = optimize(metricsPtr);

    const metrics = new Float64Array(Module.HEAPF64.buffer, optimizedPtr, 4);
    document.getElementById('cpu').textContent = metrics[0].toFixed(2);
    document.getElementById('memory').textContent = metrics[1].toFixed(2);
    document.getElementById('gpu').textContent = metrics[2].toFixed(2);
    document.getElementById('anomaly').textContent = metrics[3].toFixed(2);
}
