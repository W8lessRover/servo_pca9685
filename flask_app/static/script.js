let axisMapping = {};  // { servoIndex: axisIndex }

// Fetch servo values and update UI
async function fetchValues() {
  let res = await fetch('/get_values');
  let data = await res.json();

  // Update Output Enable button
  let button = document.getElementById('outputButton');
  if (data.enabled) {
    button.textContent = "Enabled";
    button.className = "button enabled";
  } else {
    button.textContent = "Disabled";
    button.className = "button disabled";
  }

  // Update servo sliders
  let slidersDiv = document.getElementById('sliders');
  slidersDiv.innerHTML = "";
  data.values.forEach((val, idx) => {
    let container = document.createElement("div");
    container.className = "servo-control";
    container.innerHTML = `
      <span class="servo-label">Servo ${idx}:</span>
      <input type="range" min="1000" max="2000" value="${val}" 
        oninput="updateServo(${idx}, this.value)">
      <span id="val${idx}">${val}</span>
      <select onchange="mapAxis(${idx}, this.value)">
        <option value="">-- No Axis --</option>
        <option value="0">Axis 0</option>
        <option value="1">Axis 1</option>
        <option value="2">Axis 2</option>
        <option value="3">Axis 3</option>
      </select>
    `;
    slidersDiv.appendChild(container);
  });
}

// Update a single servo
async function updateServo(ch, val) {
  document.getElementById(`val${ch}`).textContent = val;
  await fetch('/set_servo', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ channel: ch, value: val })
  });
}

// Update all servos via SETALL
async function setAllServos(val) {
  for (let i = 0; i < 16; i++) {
    document.getElementById(`val${i}`)?.textContent = val;
    let slider = document.querySelector(`input[type=range][oninput*="${i}"]`);
    if (slider) slider.value = val;
  }
  document.getElementById("allServosSlider").value = val;
  document.getElementById("allServosVal").textContent = val;

  await fetch('/set_all', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ value: val })
  });
}

// Handle the All Servos slider
async function updateAllServos(val) {
  document.getElementById("allServosVal").textContent = val;
  for (let i = 0; i < 16; i++) {
    document.getElementById(`val${i}`)?.textContent = val;
    let slider = document.querySelector(`input[type=range][oninput*="${i}"]`);
    if (slider) slider.value = val;
  }
  await fetch('/set_all', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ value: val })
  });
}

// Map joystick axes to servos
function mapAxis(servo, axis) {
  if (axis === "") {
    delete axisMapping[servo];
  } else {
    axisMapping[servo] = parseInt(axis);
  }
}

// Poll joystick
function pollGamepads() {
  let gamepads = navigator.getGamepads ? navigator.getGamepads() : [];
  if (!gamepads) return;

  for (let servo in axisMapping) {
    let axis = axisMapping[servo];
    let gp = gamepads[0];
    if (gp && gp.axes.length > axis) {
      let val = gp.axes[axis]; // -1 to 1
      let us = 1500 + val * 500;
      updateServo(parseInt(servo), Math.round(us));
    }
  }
  requestAnimationFrame(pollGamepads);
}

// Output Enable button
document.getElementById("outputButton").onclick = async () => {
  await fetch('/toggle_output', { method: 'POST' });
  fetchValues();
};

// Start polling and value refresh
setInterval(fetchValues, 500);
window.addEventListener("gamepadconnected", function(e) {
  console.log("Gamepad connected:", e.gamepad.id);
  pollGamepads();
});
