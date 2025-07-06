#pragma once

#include "esphome/core/component.h"
#include "esphome/core/optional.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "DFRobot_EnvironmentalSensor.h"

namespace esphome {
namespace dfrobot_envsensor {

class DFRobotEnvSensor : public PollingComponent, public i2c::I2CDevice {
 public:
  // Inherit constructors to allow update_interval and I2C address from YAML
  using PollingComponent::PollingComponent;
  using i2c::I2CDevice::I2CDevice;

  void setup() override;
  void update() override;

  void set_temperature_sensor(sensor::Sensor *sensor) { temperature_sensor_ = sensor; }
  void set_humidity_sensor(sensor::Sensor *sensor) { humidity_sensor_ = sensor; }
  void set_pressure_sensor(sensor::Sensor *sensor) { pressure_sensor_ = sensor; }
  void set_light_sensor(sensor::Sensor *sensor) { light_sensor_ = sensor; }
  void set_uv_index_sensor(sensor::Sensor *sensor) { uv_index_sensor_ = sensor; }
  void set_elevation_sensor(sensor::Sensor *sensor) { elevation_sensor_ = sensor; }

  void set_uv_gain(uint8_t gain) { uv_gain_ = gain; }
  void set_uv_resolution(uint8_t resolution) { uv_resolution_ = resolution; }
  void set_uv_rate(uint8_t rate) { uv_rate_ = rate; }
  

  /**
   * @brief Set a calibration multiplier on raw light readings.
   * @param gain  e.g. 1.0 (no change), 2.0 (double), 0.5 (half)
   */
  void set_light_gain(float gain) { this->light_gain_ = gain; }

  /**
   * @brief Set the actual sensor integration time (ms) for scaling.
   * @param ms  integration time in milliseconds (default: 100)
   */
  void set_light_integration_time(uint16_t ms) { this->light_integration_time_ms_ = ms; }

  
 protected:
  DFRobot_EnvironmentalSensor *sensor_{nullptr};

  optional<uint8_t> uv_gain_;
  optional<uint8_t> uv_resolution_;
  optional<uint8_t> uv_rate_;

  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
  sensor::Sensor *pressure_sensor_{nullptr};
  sensor::Sensor *light_sensor_{nullptr};
  optional<float> light_gain_;
  optional<uint16_t> light_integration_time_ms_;
  sensor::Sensor *uv_index_sensor_{nullptr};
  sensor::Sensor *elevation_sensor_{nullptr};
};

}  // namespace dfrobot_envsensor
}  // namespace esphome
