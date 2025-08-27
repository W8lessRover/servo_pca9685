let axisMapping = {};  // { servoIndex: axisIndex }

async function fetchValues() {
  let res = await fetch('/get_values');
  let data = await res.json();

  let button = document.getElementById('outputButton');
  if (data.enabled) {
    button.textContent = "Enabled";
    button.className = "button enabled";
  } else {
    button.textContent = "Disabled";
    button.className = "button disabled";
  }

  let slidersDiv = document.getElementById('sliders');
  slidersDiv.innerHTML = "";
  data.values.forEach((val, idx) => {
    let container = document.createElement("div");
    container.className = "servo-control";
    container.innerHTML = `
      <label>Servo ${idx}:</label> 
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

async function updateServo(ch, val) {
  document.getElementById(`val${ch}`).textContent = val;
  await fetch('/set_servo', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ channel: ch, value: val })
  });
}

function mapAxis(servo, axis) {
  if (axis === "") {
    delete axisMapping[servo];
  } else {
    axisMapping[servo] = parseInt(axis);
  }
}

// Toggle output button
document.getElementById("outputButton").onclick = async () => {
  await fetch('/toggle_output', { method: 'POST' });
  fetchValues();
};

// Poll joystick
function pollGamepads() {
  let gamepads = navigator.getGamepads ? navigator.getGamepads() : [];
  if (!gamepads) return;

  for (let servo in axisMapping) {
    let axis = axisMapping[servo];
    let gp = gamepads[0]; // use first gamepad
    if (gp && gp.axes.length > axis) {
      let val = gp.axes[axis]; // -1 to 1
      // Map joystick value (-1..1) to servo range (1000..2000)
      let us = 1500 + val * 500;
      updateServo(parseInt(servo), Math.round(us));
      document.querySelector(`#val${servo}`).textContent = Math.round(us);
      document.querySelector(`input[type=range][oninput*="${servo}"]`).value = Math.round(us);
    }
  }
  requestAnimationFrame(pollGamepads);
}

window.addEventListener("gamepadconnected", function(e) {
  console.log("Gamepad connected:", e.gamepad.id);
  pollGamepads();
});

setInterval(fetchValues, 1000);
