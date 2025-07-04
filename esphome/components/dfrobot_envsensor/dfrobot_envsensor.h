#include "esphome/core/component.h"
#include <Wire.h>

namespace esphome {
namespace dfrobot_envsensor {

class DFRobotEnvSensor : public Component {
 public:
  void setup() override {
    ESP_LOGI("dfrobot_envsensor", "DFRobot EnvSensor initialized");
  }

  int read(uint8_t reg, uint8_t *data, size_t len) {
    Wire.beginTransmission(0x38);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) return ESP_FAIL;
    Wire.requestFrom(0x38, (uint8_t)len);
    for (int i = 0; i < len && Wire.available(); i++) {
      data[i] = Wire.read();
    }
    return ESP_OK;
  }
};

}  // namespace dfrobot_envsensor
}  // namespace esphome
