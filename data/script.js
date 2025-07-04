const elements = {
  esp32: {
    temperature: document.getElementById("esp32-temperature"),
    uptime: document.getElementById("esp32-uptime"),
  },
  soilMoisture: document.getElementById("soil-moisture"),
  soilTemperature: document.getElementById("soil-temperature"),
  airHumidity: document.getElementById("air-humidity"),
  airTemperature: document.getElementById("air-temperature"),
  pumpSwitch: document.getElementById("pump-switch"),
  lightSwitch: document.getElementById("light-switch"),
  manualMode: document.getElementById("manual-mode"),
  autoMode: document.getElementById("auto-mode"),
  pumpTrigger: document.getElementById("pump-trigger"),
  soilThreshold: document.getElementById("soil-threshold"),
  tempThreshold: document.getElementById("temp-threshold"),
};

let isFetching = true;

function pumpTriggerVisibility() {
  if (elements.autoMode.checked) {
    elements.pumpTrigger.classList.remove("hide");
  } else {
    elements.pumpTrigger.classList.add("hide");
  }
}

function createDeviceHandler(device) {
  return async () => {
    const element = elements[`${device}Switch`];
    try {
      await fetch(`/${device}-switch`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ value: element.checked }),
      });
    } catch (error) {
      console.error(`${device} update error:`, error);
    }
  };
}

async function handleModeChange(e) {
  pumpTriggerVisibility();

  try {
    await fetch("/mode", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ value: e.target.value }),
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
  if (!isFetching) return;

  try {
    const res = await fetch("/data");
    const data = await res.json();

    elements.esp32.temperature.textContent = data.esp32.temperature + " °C";
    elements.esp32.uptime.textContent = data.esp32.uptime;

    elements.soilMoisture.textContent = data.soilMoisture + " %";
    elements.soilTemperature.textContent = data.ds18b20 + " °C";
    elements.airHumidity.textContent = data.dht.humidity + " %";
    elements.airTemperature.textContent = data.dht.temperature + " °C";

    elements.pumpSwitch.checked = data.waterPumpSwitch;
    elements.lightSwitch.checked = data.lightSwitch;

    elements.manualMode.checked = data.mode === "manual";
    elements.autoMode.checked = data.mode === "auto";

    elements.soilThreshold.value = data.soilThreshold;
    elements.tempThreshold.value = data.tempThreshold;

    pumpTriggerVisibility();
  } catch (error) {
    console.error("Error fetching data:", error);
  }
}

function init() {
  elements.pumpSwitch.addEventListener("change", createDeviceHandler("pump"));
  elements.lightSwitch.addEventListener("change", createDeviceHandler("light"));
  elements.manualMode.addEventListener("change", handleModeChange);
  elements.autoMode.addEventListener("change", handleModeChange);
  elements.soilThreshold.addEventListener("change", handleThresholdChange);
  elements.tempThreshold.addEventListener("change", handleThresholdChange);

  [elements.soilThreshold, elements.tempThreshold].forEach((element) => {
    element.addEventListener("focus", () => (isFetching = false));
  });

  [elements.soilThreshold, elements.tempThreshold].forEach((element) => {
    element.addEventListener("blur", () => (isFetching = true));
  });

  getData();
  setInterval(getData, 1000);
}

if (document.readyState !== "loading") {
  init();
} else {
  document.addEventListener("DOMContentLoaded", init);
}
