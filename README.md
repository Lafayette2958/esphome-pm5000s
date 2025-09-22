# esphome-pm5000s
external_components:
  - source: github://Lafayette2958/esphome-pm2x05@master
    components: [ pm5000s]

i2c:
  sda: GPIO21     
  scl: GPIO22
  scan: true
  frequency: 100kHz
  id: bus_i2c  

sensor:
  - platform: pm5000s
    i2c_id: bus_i2c
    address: 0x28
    update_interval: 10s
    particles_03um:
      name: "Particles >0.3µm"
    particles_05um:
      name: "Particles >0.5µm"
    particles_10um:
      name: "Particles >1.0µm"
    particles_25um:
      name: "Particles >2.5µm"
    particles_50um:
      name: "Particles >5.0µm"
    particles_100um:
      name: "Particles >10.0µm"

