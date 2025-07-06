#include "dfrobot_envsensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_envsensor {

static const char *const TAG = "dfrobot_envsensor";

void DFRobotEnvSensor::setup() {
  sensor_ = new DFRobot_EnvironmentalSensor(0x22, &Wire);
  ESP_LOGI(TAG, "Starting setup for DFRobot Environmental Sensor");

  if (sensor_->begin() != 0) {
    ESP_LOGE(TAG, "Failed to initialize DFRobot Environmental Sensor");
  } else {
    ESP_LOGI(TAG, "DFRobot Environmental Sensor initialized");
    delay(2000);  // Allow sensor to stabilize

    // Optional UV configuration
    if (this->uv_gain_.has_value())
      sensor_->setUVGain(this->uv_gain_.value());
    if (this->uv_resolution_.has_value())
      sensor_->setUVResolution(this->uv_resolution_.value());
    if (this->uv_rate_.has_value())
      sensor_->setUVMeasurementRate(this->uv_rate_.value());

   // this->set_update_interval(60000);  // Ensure update() is called every 60s
  }
}



void DFRobotEnvSensor::update() {
  if (sensor_ == nullptr)
    return;

  float t = sensor_->getTemperature(TEMP_C);
  float h = sensor_->getHumidity();
  float p = sensor_->getAtmospherePressure(HPA);
  float l = sensor_->getLuminousIntensity();
  float elev = sensor_->getElevation(); 
 
  // Updated UV calculation inline based on Python logic
  float u = 0.0f;
  uint8_t buffer[2];
  sensor_->getUVRaw(buffer);  // Wrapper must be public in DFRobot_EnvironmentalSensor
  uint16_t uv_raw = (buffer[0] << 8) | buffer[1];
  float voltage = 3.0f * uv_raw / 1024.0f;
  if (voltage <= 0.99f) {
    u = 0.0f;
  } else if (voltage >= 2.9f) {
    u = 15.0f;
  } else {
    u = (voltage - 0.99f) * (15.0f / (2.9f - 0.99f));
  }
 
  ESP_LOGD(TAG, "Read values: T=%.2f°C H=%.2f%% P=%.1f hPa L=%.1f lx UV=%.2f Elev=%.1f m", t, h, p, l, u, elev);

  if (temperature_sensor_ != nullptr)
    temperature_sensor_->publish_state(t);
  if (humidity_sensor_ != nullptr)
    humidity_sensor_->publish_state(h);
  if (pressure_sensor_ != nullptr)
    pressure_sensor_->publish_state(p);
  if (light_sensor_ != nullptr)
    light_sensor_->publish_state(l);
  if (uv_index_sensor_ != nullptr)
    uv_index_sensor_->publish_state(u);
  if (elevation_sensor_ != nullptr)
    elevation_sensor_->publish_state(elev);
}

}  // namespace dfrobot_envsensor
}  // namespace esphome
