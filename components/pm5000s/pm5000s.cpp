#include "pm5000s.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h" 

namespace esphome {
namespace pm5000s {

static const char *const TAG = "pm5000s";

void PM5000SSensor::setup() {
  uint8_t cmd[] = {0x16, 0x07, 0x02, 0x00, 0x00, 0x00, 0x13};
  if (this->write_bytes(0x50, cmd, sizeof(cmd))) {
    ESP_LOGI(TAG, "Sent start measurement command");
  } else {
    ESP_LOGW(TAG, "Failed to send start measurement command");
  }

}

void PM5000SSensor::update() {
  uint8_t data[32] = {0};
  if (!this->read_bytes(0x51, data, sizeof(data))) {
    ESP_LOGW(TAG, "Failed to read data");
    return;
  }

  if (data[0] != 0x16) {
    ESP_LOGW(TAG, "Invalid header: 0x%02X", data[0]);
    return;
  }

  uint8_t checksum = 0;
  for (int i = 0; i < 31; i++) checksum ^= data[i];
  if (checksum != data[31]) {
    ESP_LOGW(TAG, "Checksum mismatch (got 0x%02X expected 0x%02X)", data[31], checksum);
    return;
  }

  ESP_LOGI(TAG, "Status: 0x%02X", data[2]);

  uint8_t status = data[2];
  const char *status_str = "?";
  switch (status) {
    case 0x01: status_str = "Closed"; break;
    case 0x02: status_str = "Measuring"; break;
    case 0x07: status_str = "Fault"; break;
    case 0x80: status_str = "Stable"; break;
  }
  ESP_LOGI(TAG, "Sensor status: 0x%02X (%s)", status, status_str);

  auto get_u32 = [](uint8_t *buf, int idx) {
    return (uint32_t)buf[idx] << 24 | (uint32_t)buf[idx + 1] << 16 |
           (uint32_t)buf[idx + 2] << 8 | (uint32_t)buf[idx + 3];
  };

  uint32_t pm03 = get_u32(data, 7);
  uint32_t pm05 = get_u32(data, 11);
  uint32_t pm10 = get_u32(data, 15);
  uint32_t pm25 = get_u32(data, 19);
  uint32_t pm50 = get_u32(data, 23);
  uint32_t pm100 = get_u32(data, 27);

  ESP_LOGI(TAG, "Particles [pcs/L]: >0.3µm=%u >0.5µm=%u >1.0µm=%u >2.5µm=%u >5.0µm=%u >10µm=%u",
           pm03, pm05, pm10, pm25, pm50, pm100);

  if (particles_03um) particles_03um->publish_state(pm03);
  if (particles_05um) particles_05um->publish_state(pm05);
  if (particles_10um) particles_10um->publish_state(pm10);
  if (particles_25um) particles_25um->publish_state(pm25);
  if (particles_50um) particles_50um->publish_state(pm50);
  if (particles_100um) particles_100um->publish_state(pm100);
}

}  // namespace pm5000s
}  // namespace esphome
