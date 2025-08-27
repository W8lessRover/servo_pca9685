let axisMapping = {};  // { servoIndex: axisIndex }

// -------------------------------
// Create sliders once
// -------------------------------
function createSliders(numServos) {
  let slidersDiv = document.getElementById('sliders');
  slidersDiv.innerHTML = "";

  for (let idx = 0; idx < numServos; idx++) {
    let container = document.createElement("div");
    container.className = "servo-control";
    container.innerHTML = `
      <span class="servo-label">Servo ${idx}:</span>
      <input type="range" min="1000" max="2000" value="1500" id="slider${idx}">
      <span id="val${idx}">1500</span>
      <select id="axis${idx}">
        <option value="">-- No Axis --</option>
        <option value="0">Axis 0</option>
        <option value="1">Axis 1</option>
        <option value="2">Axis 2</option>
        <option value="3">Axis 3</option>
      </select>
    `;
    slidersDiv.appendChild(container);

    // Attach slider input handler
    document.getElementById(`slider${idx}`).addEventListener("input", function() {
      updateServo(idx, this.value);
    });

    // Attach axis mapping handler
    document.getElementById(`axis${idx}`).addEventListener("change", function() {
      mapAxis(idx, this.value);
    });
  }
}

// -------------------------------
// Fetch servo values and update sliders
// -------------------------------
async function fetchValues() {
  let res = await fetch('/get_values');
  let data = await res.json();

  data.values.forEach((val, idx) => {
    let slider = document.getElementById(`slider${idx}`);
    let valSpan = document.getElementById(`val${idx}`);
    if (slider && valSpan) {
      slider.value = val;
      valSpan.textContent = val;
    }
  });

  // Update All Servos slider to average value
  let avgVal = Math.round(data.values.reduce((a,b) => a+b)/data.values.length);
  document.getElementById("allServosSlider").value = avgVal;
  document.getElementById("allServosVal").textContent = avgVal;
}

// -------------------------------
// Update a single servo
// -------------------------------
async function updateServo(ch, val) {
  document.getElementById(`val${ch}`).textContent = val;
  await fetch('/set_servo', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ channel: ch, value: val })
  });
}

// -------------------------------
// Update all servos
// -------------------------------
async function updateAllServos(val) {
  for (let i = 0; i < 16; i++) {
    let slider = document.getElementById(`slider${i}`);
    let valSpan = document.getElementById(`val${i}`);
    if (slider && valSpan) {
      slider.value = val;
      valSpan.textContent = val;
    }
  }

  document.getElementById("allServosSlider").value = val;
  document.getElementById("allServosVal").textContent = val;

  await fetch('/set_all', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ value: val })
  });
}

// -------------------------------
// Map joystick axes to servos
// -------------------------------
function mapAxis(servo, axis) {
  if (axis === "") {
    delete axisMapping[servo];
  } else {
    axisMapping[servo] = parseInt(axis);
  }
}

// -------------------------------
// Poll joystick
// -------------------------------
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

// -------------------------------
// Enable / Disable Buttons
// -------------------------------
document.getElementById("enableButton").addEventListener("click", async () => {
  await fetch('/enable_output', { method: 'POST' });
  updateOutputButtons(true);
});

document.getElementById("disableButton").addEventListener("click", async () => {
  await fetch('/disable_output', { method: 'POST' });
  updateOutputButtons(false);
});

function updateOutputButtons(enabled) {
  document.getElementById("enableButton").className = enabled ? "button enabled" : "button disabled";
  document.getElementById("disableButton").className = enabled ? "button disabled" : "button enabled";
}

// -------------------------------
// All Min/Mid/Max Buttons
// -------------------------------
document.getElementById("allMinBtn").addEventListener("click", () => updateAllServos(1000));
document.getElementById("allMidBtn").addEventListener("click", () => updateAllServos(1500));
document.getElementById("allMaxBtn").addEventListener("click", () => updateAllServos(2000));

// All Servos slider
document.getElementById("allServosSlider").addEventListener("input", function() {
  updateAllServos(this.value);
});

// -------------------------------
// Initialize
// -------------------------------
createSliders(16);            // create all servo sliders once
setInterval(fetchValues, 500); // update values every 500ms

window.addEventListener("gamepadconnected", function(e) {
  console.log("Gamepad connected:", e.gamepad.id);
  pollGamepads();
});
