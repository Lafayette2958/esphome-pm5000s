#include "esphome.h"

class Gasboard7500OPCSensor : public Component, public UARTDevice {
public:
  Sensor *gas_flow = new Sensor();
  Sensor *gas_temperature = new Sensor();


Gasboard7500OPCSensor(UARTComponent *parent) : UARTDevice(parent) {}

float get_setup_priority() const override { return esphome::setup_priority::BUS; }


void setup() override {
  uint8_t command[] = {0x11, 0x01, 0x07, 0xE7};
  this->write_array(command, sizeof(command));
  ESP_LOGD("gasboard", "Send Response  mode to toggle : %02X %02X %02X %02X", command[0], command[1], command[2], command[3]);

}

void loop() override {

    while (available() >= 12) {
    if (peek() != 0x16) {
    read(); 
    continue;
}
    uint8_t data[12];
    for (int i = 0; i < 12; i++) {
    data[i] = read();
}

    if (data[2] != 0x01) {
ESP_LOGW("gasboard", "Unknown CMD : 0x%02X", data[2]);
    continue;
}
    uint8_t checksum = 0;
    for (int i = 0; i < 11; i++) {
    checksum += data[i];
}
    checksum = 256 - checksum;  
    if (data[11] != checksum) {
ESP_LOGW("gasboard", "Checksum mismatch: expected 0x%02X, got 0x%02X", checksum, data[11]);
    continue;
        
}

uint16_t flow_raw = (data[5] << 8) | data[6];
uint16_t temp_raw = (data[7] << 8) | data[8];

float flow = flow_raw / 100.0f;
float temperature = temp_raw / 10.0f;

gas_flow->publish_state(flow);
gas_temperature->publish_state(temperature);

ESP_LOGI("gasboard", "Flow: %.2f L/min", flow);
ESP_LOGI("gasboard", "Temp: %.1f °C", temperature);
ESP_LOGD("gasboard", "Flow: %.2f L/min, Temp: %.1f °C", flow, temperature);


}
//else {read();}         
}     
};
