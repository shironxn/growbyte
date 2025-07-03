const elements = {
  soilMoisture: document.getElementById("soil-moisture"),
  soilTemperature: document.getElementById("soil-temperature"),
  airHumidity: document.getElementById("air-humidity"),
  airTemperature: document.getElementById("air-temperature"),
  pumpSwitch: document.getElementById("pump-switch"),
  lightSwitch: document.getElementById("light-switch"),
  manualMode: document.getElementById("manual-mode"),
  autoMode: document.getElementById("auto-mode"),
  autoConfig: document.getElementById("auto-config"),
  soilThreshold: document.getElementById("soil-threshold"),
  tempThreshold: document.getElementById("temp-threshold"),
};

async function handleDeviceToggle(device) {
  const element = elements[`${device}Switch`];
  console.log(device);
  try {
    await fetch(`/${device}-switch`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ value: element.checked }),
    });
  } catch (error) {
    console.error(`${device} update error:`, error);
  }
}

async function handleModeChange(e) {
  const mode = e.target.value;
  try {
    elements.autoConfig.classList.toggle("hide", mode === "manual");
    await fetch("/mode", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ value: mode }),
    });
  } catch (error) {
    console.error("Mode change error:", error);
  }
}

async function handleThresholdChange() {
  try {
    await fetch("/pump-trigger", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        soilThreshold: parseFloat(elements.soilThreshold.value),
        tempThreshold: parseFloat(elements.tempThreshold.value),
      }),
    });
  } catch (error) {
    console.error("Threshold update error:", error);
  }
}

async function getData() {
  try {
    const res = await fetch("/data");
    const data = await res.json();

    elements.soilMoisture.textContent = data.soilMoisture;
    elements.soilTemperature.textContent = data.ds18b20;
    elements.airHumidity.textContent = data.dht11.humidity;
    elements.airTemperature.textContent = data.dht11.temperature;

    elements.pumpSwitch.checked = data.waterPumpSwitch;
    elements.lightSwitch.checked = data.lightSwitch;

    const isAutoMode = data.mode === "auto";
    elements.manualMode.checked = !isAutoMode;
    elements.autoMode.checked = isAutoMode;
    elements.autoConfig.classList.toggle("hide", !isAutoMode);

    elements.soilThreshold.value = data.soilThreshold;
    elements.tempThreshold.value = data.tempThreshold;
  } catch (error) {
    console.error("Error fetching data:", error);
  }
}

function init() {
  elements.pumpSwitch.addEventListener("change", handleDeviceToggle("pump"));
  elements.lightSwitch.addEventListener("change", handleDeviceToggle("light"));
  elements.manualMode.addEventListener("change", handleModeChange);
  elements.autoMode.addEventListener("change", handleModeChange);
  elements.soilThreshold.addEventListener("change", handleThresholdChange);
  elements.tempThreshold.addEventListener("change", handleThresholdChange);

  getData();
  setInterval(getData, 5000);
}

if (document.readyState !== "loading") {
  init();
} else {
  document.addEventListener("DOMContentLoaded", init);
}
