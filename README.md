# esphome-pm5000s
external_components:
  - source:
      type: git
      url: https://github.com/Lafayette2958/esphome-pm5000s
    components: [pm5000s]

i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true

sensor:
  - platform: pm5000s
    address: 0x28
    update_interval: 10s
    particles_03um:
      name: "Particles >0.3µm"
      unit_of_measurement: "pcs/L"
      accuracy_decimals: 2
    particles_05um:
      name: "Particles >0.5µm"
      unit_of_measurement: "pcs/L"
      accuracy_decimals: 2
    particles_10um:
      name: "Particles >1.0µm"
      unit_of_measurement: "pcs/L"
      accuracy_decimals: 2
    particles_25um:
      name: "Particles >2.5µm"
      unit_of_measurement: "pcs/L"
      ccuracy_decimals: 2
    particles_50um:
      name: "Particles >5.0µm"
      unit_of_measurement: "pcs/L"
      accuracy_decimals: 2
    particles_100um:
      name: "Particles >10.0µm"
      unit_of_measurement: "pcs/L"
      accuracy_decimals: 2

