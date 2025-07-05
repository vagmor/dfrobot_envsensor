# ESPHome Component: DFRobot Environmental Sensor (SEN0500 / SEN0501)

[![ESPHome](https://img.shields.io/badge/ESPHome-compatible-blue.svg)](https://esphome.io/)

This is a custom [ESPHome](https://esphome.io/) component for interfacing with the **DFRobot SEN0500 / SEN0501 Environmental Sensor** via I²C.

It exposes the following sensors:
- 🌡️ Temperature (°C)
- 💧 Humidity (%)
- 🌬️ Barometric Pressure (hPa)
- 🌞 Light Intensity (lux)
- ☀️ UV Index (calculated from raw ADC)
- 🗻 Elevation (meters, derived from pressure)

## 🧪 Sensor Features

This sensor module internally integrates:
- SHTC3 (temperature and humidity)
- BMP280 (pressure)
- LTR390 (UV and ambient light)

## 📦 Installation

Clone or copy this repository to your ESPHome `/config/esphome/components/dfrobot_envsensor/` directory:

```bash
mkdir -p config/esphome/components/dfrobot_envsensor
git clone https://github.com/vagmor/dfrobot_envsensor.git config/esphome/components/dfrobot_envsensor
```

Or add as a git submodule in your ESPHome setup:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/vagmor/dfrobot_envsensor
```

## ✨ Example ESPHome YAML

```yaml
i2c:
  sda: D2
  scl: D1
  scan: true

sensor:
  - platform: dfrobot_envsensor
    address: 0x22
    temperature:
      name: "Temperature"
      accuracy_decimals: 2
    humidity:
      name: "Humidity"
      accuracy_decimals: 2
    pressure:
      name: "Pressure"
    light:
      name: "Light"
    uv_index:
      name: "UV Index"
    elevation:
      name: "Elevation"
    update_interval: 60s
```

## ⚙️ Notes

- Elevation is calculated from barometric pressure using a standard formula.
- UV Index is derived using DFRobot’s published LTR390 transfer function (voltage → index).
- A short stabilization delay is included during setup (`delay(2000)`).
- You can optionally calibrate lux or UV based on your setup using offset or multiplier in YAML.

## 🧾 Acknowledgements

- Sensor logic based on official Python library from DFRobot:
  https://github.com/DFRobot/DFRobot_EnvironmentalSensor
- Sensor calibration formulas adapted directly from upstream documentation.

## 🛠️ Contributing

Feel free to open issues or pull requests. This is an open project for integrating DFRobot_EnvironmentalSensor SEN0501 DFRobot Gravity Multifunctional Environmental Sensor data with Home Assistant.

---

© 2025 [@vagmor](https://github.com/vagmor)
