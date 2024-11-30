function createHelixStack(scene, layers) {
    const radius = 5; // Radius of the helix
    const heightStep = 2; // Distance between each layer
    const helixGroup = new THREE.Group();

    layers.forEach((layer, index) => {
        const angle = index * (Math.PI / 6); // Adjust angle for stacking
        const x = radius * Math.cos(angle);
        const z = radius * Math.sin(angle);
        const y = index * heightStep;

        // Create a sphere to represent a layer
        const geometry = new THREE.SphereGeometry(0.5, 32, 32);
        const material = new THREE.MeshStandardMaterial({ color: 0x00ffcc });
        const sphere = new THREE.Mesh(geometry, material);

        sphere.position.set(x, y, z);

        // Add dynamic labels
        const label = createText(`Layer ${index + 1}`, x, y + 0.5, z);
        helixGroup.add(label);

        helixGroup.add(sphere);
    });

    scene.add(helixGroup);
}

// Helper function for adding labels
function createText(text, x, y, z) {
    const loader = new THREE.FontLoader();
    const material = new THREE.MeshBasicMaterial({ color: 0xffffff });

    loader.load('path/to/font.json', font => {
        const geometry = new THREE.TextGeometry(text, {
            font: font,
            size: 0.2,
            height: 0.1
        });

        const mesh = new THREE.Mesh(geometry, material);
        mesh.position.set(x, y, z);
        return mesh;
    });
}
