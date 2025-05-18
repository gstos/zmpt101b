# ZMPT101B Sensor for ESPHome

External component for measuring AC RMS voltage using the ZMPT101B analog sensor.

## Usage

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/gstos/zmpt101b
      ref: main
    components: [zmpt101b]

sensor:
  - platform: zmpt101b
    pin: 34
    name: "Voltage Sensor"
    sensitivity: 1.5
    frequency: 60
