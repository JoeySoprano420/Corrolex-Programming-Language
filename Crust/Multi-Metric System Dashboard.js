function createAdvancedHologram(scene) {
    const metrics = ['CPU', 'GPU', 'Memory', 'Disk'];
    metrics.forEach((metric, index) => {
        const geometry = new THREE.BoxGeometry(1, Math.random() * 5, 1);
        const material = new THREE.MeshStandardMaterial({ color: 0x00ff00 });
        const bar = new THREE.Mesh(geometry, material);

        bar.position.set(index * 2 - 4, 0, 0);
        scene.add(bar);

        // Simulate metric updates
        setInterval(() => {
            bar.scale.y = Math.random() * 5 + 1;
        }, 1000);
    });
}
