const NUM_SERVOS = 16;
let servoValues = Array(NUM_SERVOS).fill(1500);
const slidersContainer = document.getElementById("slidersContainer");
let outputsEnabled = false;

// Create sliders dynamically
for (let i = 0; i < NUM_SERVOS; i++) {
    const div = document.createElement("div");
    div.className = "servo-slider";
    div.innerHTML = `
        <span class="servo-label">Servo ${i}:</span>
        <input type="range" min="1000" max="2000" value="1500" id="slider${i}">
        <span class="servo-value" id="value${i}">1500</span>
    `;
    slidersContainer.appendChild(div);

    const slider = document.getElementById(`slider${i}`);
    slider.oninput = () => {
        const val = slider.value;
        document.getElementById(`value${i}`).innerText = val;
        servoValues[i] = val;
        fetch('/set_servo', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ channel: i, value: parseInt(val) })
        });
    };
}

// Enable button: read current values first
document.getElementById("enableBtn").onclick = async () => {
    const res = await fetch('/get_values');
    const data = await res.json();
    data.values.forEach((val, i) => {
        servoValues[i] = val;
        document.getElementById(`slider${i}`).value = val;
        document.getElementById(`value${i}`).innerText = val;
    });

    // Send enable command
    await fetch('/enable_output', { method: 'POST' });
    outputsEnabled = true;
    document.getElementById("enableBtn").className = "button red";
    document.getElementById("disableBtn").className = "button green";
};

// Disable button: read current values first
document.getElementById("disableBtn").onclick = async () => {
    const res = await fetch('/get_values');
    const data = await res.json();
    data.values.forEach((val, i) => {
        servoValues[i] = val;
        document.getElementById(`slider${i}`).value = val;
        document.getElementById(`value${i}`).innerText = val;
    });

    // Send disable command
    await fetch('/disable_output', { method: 'POST' });
    outputsEnabled = false;
    document.getElementById("enableBtn").className = "button red";
    document.getElementById("disableBtn").className = "button green";
};

// Set all servos to a specific value
function setAll(val) {
    for (let i = 0; i < NUM_SERVOS; i++) {
        document.getElementById(`slider${i}`).value = val;
        document.getElementById(`value${i}`).innerText = val;
        servoValues[i] = val;
    }
    fetch('/set_all', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ value: parseInt(val) })
    });
}

// Periodically fetch current servo values from backend
setInterval(() => {
    fetch('/get_values')
        .then(res => res.json())
        .then(data => {
            data.values.forEach((val, i) => {
                servoValues[i] = val;
                document.getElementById(`slider${i}`).value = val;
                document.getElementById(`value${i}`).innerText = val;
            });
        });
}, 200); // update every 200ms
