const soilMoisture = document.getElementById("soil-moisture");
const ds18b20 = document.getElementById("ds18b20");
const dht11 = document.getElementById("dht11");
const esp32 = document.getElementById("esp32");

const pumpSwitch = document.getElementById("pump-switch");
const lightSwitch = document.getElementById("light-switch");
const modeRadios = document.querySelectorAll("input[name='mode']");

async function fetchData() {
  try {
    const res = await fetch("/data");
    const data = await res.json();

    soilMoisture.textContent = data.soilMoisture + " %";
    ds18b20.textContent = data.ds18b20 + " °C";
    dht11.textContent =
      data.dht.temperature + " °C / " + data.dht.humidity + " %";
    esp32.textContent = data.esp32.temperature + " °C";

    pumpSwitch.checked = data.waterPumpSwitch;
    lightSwitch.checked = data.lightSwitch;
    document.querySelector(`input[name="mode"][value="${data.mode}"]`).checked =
      true;
  } catch (error) {
    console.error("Error fetching data:", error);
  }
}

pumpSwitch.addEventListener("change", () => {
  fetch("/pump-switch", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({ value: pumpSwitch.checked }),
  });
});

lightSwitch.addEventListener("change", () => {
  fetch("/light-switch", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({ value: lightSwitch.checked }),
  });
});

modeRadios.forEach((radio) => {
  radio.addEventListener("change", () => {
    fetch("/mode", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ value: radio.value }),
    });
  });
});

fetchData();
setInterval(fetchData, 5000);
