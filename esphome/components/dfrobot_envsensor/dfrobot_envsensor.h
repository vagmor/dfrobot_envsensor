#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "DFRobot_EnvironmentalSensor.h"

namespace esphome {
namespace dfrobot_envsensor {

class DFRobotEnvSensor : public PollingComponent {
 public:
  DFRobotEnvSensor() : PollingComponent(60000) {}

  void setup() override;
  void update() override;

  // Sensor setters
  void set_temperature_sensor(sensor::Sensor *sensor) { temperature_sensor_ = sensor; }
  void set_humidity_sensor(sensor::Sensor *sensor) { humidity_sensor_ = sensor; }
  void set_pressure_sensor(sensor::Sensor *sensor) { pressure_sensor_ = sensor; }
  void set_light_sensor(sensor::Sensor *sensor) { light_sensor_ = sensor; }
  void set_uv_index_sensor(sensor::Sensor *sensor) { uv_index_sensor_ = sensor; }
  void set_elevation_sensor(sensor::Sensor *sensor) { elevation_sensor_ = sensor; }

 protected:
  DFRobot_EnvironmentalSensor *sensor_{nullptr};

  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
  sensor::Sensor *pressure_sensor_{nullptr};
  sensor::Sensor *light_sensor_{nullptr};
  sensor::Sensor *uv_index_sensor_{nullptr};
  sensor::Sensor *elevation_sensor_{nullptr};
};

}  // namespace dfrobot_envsensor
}  // namespace esphome
