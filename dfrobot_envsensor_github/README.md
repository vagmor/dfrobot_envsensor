# ESPHome DFRobot EnvSensor Component

This is a custom ESPHome external component for the DFRobot environmental sensor module (SEN0501), which includes:

- SHTC3 (Temperature and Humidity)
- BMP280 (Pressure)
- VEML7700 (Light)
- LTR390 (UV)

The sensor has an onboard MCU that outputs all values via I2C at address `0x38`.

## Installation

In your ESPHome YAML, add:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/yourusername/dfrobot_envsensor
dfrobot_envsensor:
  id: env_i2c
```

## Usage

Use an `interval:` block and call `read(...)` to pull raw bytes, then parse into values:

```yaml
interval:
  - interval: 10s
    then:
      - lambda: |-
          uint8_t buf[40];
          if (id(env_i2c).read(0x00, buf, 40) == esphome::ESP_OK) {
            // parse buf[] into float values and publish
          }
```

## License

MIT
