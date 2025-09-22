#ifndef PM5000S_H
#define PM5000S_H

#include "esphome.h"

class PM5000SSensor : public PollingComponent, public i2c::I2CDevice {
 public:
  Sensor *particles_03um = new Sensor();
  Sensor *particles_05um = new Sensor();
  Sensor *particles_10um = new Sensor();
  Sensor *particles_25um = new Sensor();
  Sensor *particles_50um = new Sensor();
  Sensor *particles_100um = new Sensor();

  PM5000SSensor(uint8_t address = 0x28, i2c::I2CBus *bus = nullptr) : PollingComponent(5000) {
    this->set_i2c_address(address);
    if (bus != nullptr) {
      this->set_i2c_bus(bus);
    }
  }

  void setup() override;
  void update() override;
};

#endif  // PM5000S_H
