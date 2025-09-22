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
float calibration_factor = 1.0f;  // Default factor

void set_calibration_factor(float factor) {
  calibration_factor = factor;
}

void setup() override {
    uint8_t cmd[] = {0x16, 0x07, 0x02, 0x00, 0x00, 0x00, 0x13};
    if (this->write_bytes(0x50, cmd, sizeof(cmd))) {
      ESP_LOGI("pm5000s", "Sent start measurement command");
    } else {
      ESP_LOGW("pm5000s", "Failed to send start measurement command");
    }
    delay(1000);  
  }

void update() override {
    uint8_t data[32] = {0};
    if (!this->read_bytes(0x51, data, sizeof(data))) {
      ESP_LOGW("pm5000s", "Failed to read data");
      return;
  }

    if (data[0] != 0x16) {
      ESP_LOGW("pm5000s", "Invalid header: 0x%02X", data[0]);
      return;
    }

uint8_t checksum = 0;
    for (int i = 0; i < 31; i++) checksum ^= data[i];
    if (checksum != data[31]) {
      ESP_LOGW("pm5000s", "Checksum mismatch (got 0x%02X expected 0x%02X)",
               data[31], checksum);
      return;
    }
    ESP_LOGI("pm5000s", "Status: 0x%02X", data[2]);

uint8_t status = data[2];
    const char *status_str = "?";
    switch (status) {
      case 0x01: status_str = "Closed"; break;
      case 0x02: status_str = "Measuring"; break;
      case 0x07: status_str = "Fault"; break;
      case 0x80: status_str = "Stable"; break;
    }
ESP_LOGI("pm5000s", "Sensor status: 0x%02X (%s)", status, status_str);


    
auto get_u32 = [](uint8_t *buf, int idx) { return (uint32_t)buf[idx] << 24 |(uint32_t)buf[idx + 1] << 16 | (uint32_t)buf[idx + 2] << 8 | (uint32_t)buf[idx + 3];};

//uint32_t pm03  = get_u32(data, 8);
//uint32_t pm05  = get_u32(data, 12);
//uint32_t pm10  = get_u32(data, 16);
//uint32_t pm25  = get_u32(data, 20);
//uint32_t pm50  = get_u32(data, 24);
//uint32_t pm100 = get_u32(data, 28); 

uint32_t pm03  = get_u32(data, 7);
uint32_t pm05  = get_u32(data, 11);
uint32_t pm10  = get_u32(data, 15);
uint32_t pm25  = get_u32(data, 19);
uint32_t pm50  = get_u32(data, 23);
uint32_t pm100 = get_u32(data, 27);


ESP_LOGI("pm5000s", "Particles [pcs/L]: >0.3µm=%u >0.5µm=%u >1.0µm=%u >2.5µm=%u >5.0µm=%u >10µm=%u", pm03, pm05, pm10, pm25, pm50, pm100);

    particles_03um->publish_state(pm03);
    particles_05um->publish_state(pm05);
    particles_10um->publish_state(pm10);
    particles_25um->publish_state(pm25);
    particles_50um->publish_state(pm50);
    particles_100um->publish_state(pm100);
  
    
  }
};
