
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace pm5000s {

class PM5000SSensor : public PollingComponent, public i2c::I2CDevice {
 public:
  sensor::Sensor *particles_03um{nullptr};
  sensor::Sensor *particles_05um{nullptr};
  sensor::Sensor *particles_10um{nullptr};
  sensor::Sensor *particles_25um{nullptr};
  sensor::Sensor *particles_50um{nullptr};
  sensor::Sensor *particles_100um{nullptr};


explicit PM5000SSensor(uint8_t address = 0x28) : PollingComponent(10000), i2c::I2CDevice() {
  this->set_i2c_address(address);
}


  void setup() override;
  void update() override;

  void set_particles_03um_sensor(sensor::Sensor *sensor) { this->particles_03um = sensor; }
  void set_particles_05um_sensor(sensor::Sensor *sensor) { this->particles_05um = sensor; }
  void set_particles_10um_sensor(sensor::Sensor *sensor) { this->particles_10um = sensor; }
  void set_particles_25um_sensor(sensor::Sensor *sensor) { this->particles_25um = sensor; }
  void set_particles_50um_sensor(sensor::Sensor *sensor) { this->particles_50um = sensor; }
  void set_particles_100um_sensor(sensor::Sensor *sensor) { this->particles_100um = sensor; }
};

}  // namespace pm5000s
}  // namespace esphome
