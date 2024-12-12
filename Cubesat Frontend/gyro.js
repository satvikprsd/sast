// WebSocket connection to server
const socket = new WebSocket('ws://localhost:3000');

// 3D Scene Setup
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// Create a 3D cube
const geometry = new THREE.BoxGeometry();
const material = new THREE.MeshBasicMaterial({ color: 0x84aafa });
const cube = new THREE.Mesh(geometry, material);
cube.scale.set(2, 2, 2);
scene.add(cube);

// Position the camera
camera.position.z = 5;

// WebSocket Data Handling: Update 3D Model with [x, y, z]
let xData = 0, yData = 0, zData = 0;
socket.addEventListener('message', (event) => {
    const data = JSON.parse(event.data); // Assume data is in JSON format
    console.log(data);
    // Extract [x, y, z] from the last index of the array (e.g., [0, 0, 0, 0, 0, [x, y, z]])
    [xData, yData, zData] = [data[data.length-3],data[data.length-2],data[data.length-1]]; // Extract x, y, z values
    console.log(xData,yData,zData);

});

// Animation loop to update the 3D cube's position based on the WebSocket data
function animate() {
    requestAnimationFrame(animate);

    // Update the cube's position using the [x, y, z] data
    cube.rotation.x += xData * 0.01; // Rotate around x-axis
    cube.rotation.y += yData * 0.01; // Rotate around y-axis
    cube.rotation.z += zData * 0.01; // Rotate around z-axis

    renderer.render(scene, camera);
}

// Start animation loop
animate();
