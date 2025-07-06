# ESPHome Component: DFRobot Environmental Sensor (SEN0500 / SEN0501)

[![ESPHome](https://img.shields.io/badge/ESPHome-compatible-blue.svg)](https://esphome.io/)

This is a custom [ESPHome](https://esphome.io/) component for interfacing with the **DFRobot SEN0500 / SEN0501 Environmental Sensor** via I²C.

It exposes the following sensors:
- 🌡️ Temperature (°C)
- 💧 Humidity (%)
- 🌬️ Barometric Pressure (hPa)
- 🌞 Light Intensity (lux)
    * `light_gain` multiplier
    * `light_integration_time` scaling
- ☀️ UV Index (calculated from raw ADC)
- 🗻 Elevation (meters, derived from pressure)

## 🧪 Sensor Features

This sensor module internally integrates:
- SHTC3 (temperature and humidity)
- BMP280 (pressure)
- LTR390 (UV Index)
- VEML7700 (Ambient light)

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

## 🔧 Configuration Options

All fields are under your `sensor:` section.

| Option                   | Type      | Default | Description                                                                                 |
| ------------------------ | --------- | ------- | ------------------------------------------------------------------------------------------- |
| `address`                | `int`     | `0x22`  | I²C address of the EnvSensor (0x20–0xF7)                                                     |
| `update_interval`        | `time`    | `60s`   | Polling interval                                                                            |
| `temperature`            | `sensor`  | —       | Declare a temperature sensor (°C)                                                           |
| `humidity`               | `sensor`  | —       | Declare a humidity sensor (%)                                                               |
| `pressure`               | `sensor`  | —       | Declare a pressure sensor (hPa)                                                              |
| `light`                  | `sensor`  | —       | Declare a light (lux) sensor                                                                |
| `uv_index`               | `sensor`  | —       | Declare a UV index sensor                                                                   |
| `elevation`              | `sensor`  | —       | Declare an elevation sensor (m)                                                             |
| `light_gain`             | `float`   | `1.0`   | (Optional) multiplier to calibrate raw lux (e.g. `1.0`, `2.0`, `0.5`)                       |
| `light_integration_time` | `int(ms)` | `100`   | (Optional) actual sensor ALS integration time in milliseconds (e.g. `100`, `200`, `400`)    |
| `uv_gain`                | `int`     | —       | (Optional) write UV gain register (0–255)                                                   |
| `uv_resolution`          | `int`     | —       | (Optional) write UV resolution register                                                    |
| `uv_rate`                | `int`     | —       | (Optional) write UV measurement rate register                                              |

> **Note:** `light_gain` and `light_integration_time` adjust the raw ALS register before conversion to lux. Useful when matching a reference meter.

---

## 🔍 Example YAML

```yaml
sensor:
  - platform: dfrobot_envsensor
    id: my_env_sensor
    address: 0x22
    update_interval: 60s

    temperature:
      name: "Grow Tent Temperature"
      accuracy_decimals: 1

    humidity:
      name: "Grow Tent Humidity"
      accuracy_decimals: 1

    pressure:
      name: "Grow Tent Pressure"
      accuracy_decimals: 1

    light:
      name: "Grow Tent Light"
      accuracy_decimals: 0

    uv_index:
      name: "Grow Tent UV Index"
      accuracy_decimals: 1

    elevation:
      name: "Grow Tent Elevation"
      accuracy_decimals: 1

    # Calibration overrides (optional)
    light_gain: 1.25
    light_integration_time: 200  # Sensor default is 100 ms

    # UV tuning (optional)
    uv_gain: 18
    uv_resolution: 20
    uv_rate: 255
````
## 🔍 Under the Hood

1. **I²C Reads**: Uses `readReg(REG_…)` to fetch raw registers for all measurements.
2. **Lux Calibration**:

   * `getLuminousRaw()` returns the raw 16‑bit ALS register.
   * `lux = raw * light_gain * (integration_time_ms / 100.0)`
   * Applies sensor‑specific polynomial conversion to lux.
3. **UV Index**: Maps raw UV register → voltage → 0–15 index via published formula.
4. **Elevation**: Computes via barometric formula:

   ```cpp
   elev = 44330 * (1.0f - pow(pressure_hPa / 1013.25f, 0.1903f));
   ```
---

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




* **UV Index** (0–15 scale) with optional UV sensor register tuning
* **Elevation** (m) via barometric calculation
* Configurable I²C address & poll interval

---






---




## 📜 License

MIT © vagmor
