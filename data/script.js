const elements = {
  themeToggle: document.getElementById("theme-toggle"),
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

const sunIcon = `<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-sun-fill" viewBox="0 0 16 16">
  <path d="M8 12a4 4 0 1 0 0-8 4 4 0 0 0 0 8M8 0a.5.5 0 0 1 .5.5v2a.5.5 0 0 1-1 0v-2A.5.5 0 0 1 8 0m0 13a.5.5 0 0 1 .5.5v2a.5.5 0 0 1-1 0v-2A.5.5 0 0 1 8 13m8-5a.5.5 0 0 1-.5.5h-2a.5.5 0 0 1 0-1h2a.5.5 0 0 1 .5.5M3 8a.5.5 0 0 1-.5.5h-2a.5.5 0 0 1 0-1h2A.5.5 0 0 1 3 8m10.657-5.657a.5.5 0 0 1 0 .707l-1.414 1.415a.5.5 0 1 1-.707-.708l1.414-1.414a.5.5 0 0 1 .707 0m-9.193 9.193a.5.5 0 0 1 0 .707L3.05 13.657a.5.5 0 0 1-.707-.707l1.414-1.414a.5.5 0 0 1 .707 0m9.193 2.121a.5.5 0 0 1-.707 0l-1.414-1.414a.5.5 0 0 1 .707-.707l1.414 1.414a.5.5 0 0 1 0 .707M4.464 4.465a.5.5 0 0 1-.707 0L2.343 3.05a.5.5 0 1 1 .707-.707l1.414 1.414a.5.5 0 0 1 0 .708"/>
</svg>`;

const moonIcon = `<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-moon-stars-fill" viewBox="0 0 16 16">
  <path d="M6 .278a.77.77 0 0 1 .08.858 7.2 7.2 0 0 0-.878 3.46c0 4.021 3.278 7.277 7.318 7.277q.792-.001 1.533-.16a.79.79 0 0 1 .81.316.73.73 0 0 1-.031.893A8.35 8.35 0 0 1 8.344 16C3.734 16 0 12.286 0 7.71 0 4.266 2.114 1.312 5.124.06A.75.75 0 0 1 6 .278"/>
  <path d="M10.794 3.148a.217.217 0 0 1 .412 0l.387 1.162c.173.518.579.924 1.097 1.097l1.162.387a.217.217 0 0 1 0 .412l-1.162.387a1.73 1.73 0 0 0-1.097 1.097l-.387 1.162a.217.217 0 0 1-.412 0l-.387-1.162A1.73 1.73 0 0 0 9.31 6.593l-1.162-.387a.217.217 0 0 1 0-.412l1.162-.387a1.73 1.73 0 0 0 1.097-1.097zM13.863.099a.145.145 0 0 1 .274 0l.258.774c.115.346.386.617.732.732l.774.258a.145.145 0 0 1 0 .274l-.774.258a1.16 1.16 0 0 0-.732.732l-.258.774a.145.145 0 0 1-.274 0l-.258-.774a1.16 1.16 0 0 0-.732-.732l-.774-.258a.145.145 0 0 1 0-.274l.774-.258c.346-.115.617-.386.732-.732z"/>
</svg>`;

function theme() {
  const getTheme = localStorage.getItem("theme") || "dark";
  document.documentElement.classList.add(getTheme);
  document.documentElement.classList.remove(getTheme === "dark" ? "light" : "dark");

  elements.themeToggle.innerHTML = getTheme === "dark" ? sunIcon : moonIcon;
}

function themeToggle() {
  const isDark = document.documentElement.classList.contains("dark");

  document.documentElement.classList.toggle("dark", !isDark);
  document.documentElement.classList.toggle("light", isDark);

  localStorage.setItem("theme", isDark ? "light" : "dark");

  elements.themeToggle.innerHTML = isDark ? moonIcon : sunIcon;
}

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
  theme(); 

  elements.themeToggle.addEventListener("click", themeToggle);
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
