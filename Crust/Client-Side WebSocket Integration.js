const socket = new WebSocket('ws://localhost:8080');

socket.onopen = () => {
    console.log('Connected to the server.');
    socket.send('Requesting system metrics...');
};

socket.onmessage = event => {
    const metrics = JSON.parse(event.data);
    updateMetrics(metrics); // Update the UI with metrics
};
