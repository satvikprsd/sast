const express = require('express');
const WebSocket = require('ws');
const { SerialPort, ReadlineParser } = require('serialport');

const app = express();
const PORT = 3000;
    
app.use(express.static('public'));

const server = app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});

const wss = new WebSocket.Server({ server });

function broadcast(data) {
    wss.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(data);
        }
    });
}

const serialPort = new SerialPort({ path: 'COM7', baudRate: 115200 }); // Adjust port if needed
const parser = serialPort.pipe(new ReadlineParser({ delimiter: '\r\n' }));

// Assume data format: "30, 45, 28, 50, 60, 70"
parser.on('data', (data) => {
    console.log(`Received: ${data}`);
    
    // Assuming data is something like "23.4, 45.1, 67.8, 34.9, 54.6, 78.9"
    const sensorValues = data.split(',').map(value => value.trim()); // Split and trim the values
    broadcast(JSON.stringify(sensorValues)); // Send as a JSON string
});

