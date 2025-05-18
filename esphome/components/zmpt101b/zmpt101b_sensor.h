#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

#if defined(AVR)
  #define ADC_SCALE 1023.0f
  #define VREF 5.0f
#elif defined(ESP8266)
  #define ADC_SCALE 1023.0f
  #define VREF 3.3f
#elif defined(ESP32)
  #define ADC_SCALE 4095.0f
  #define VREF 3.3f
#endif

namespace esphome {
    namespace zmpt101b {

        class ZMPT101BSensor : public sensor::Sensor, public PollingComponent {
        public:
            explicit ZMPT101BSensor(uint8_t pin);

            void setup() override;
            void update() override;
            void dump_config() override;

            void set_sensitivity(float sensitivity) { this->sensitivity_ = sensitivity; }
            void set_frequency(uint16_t frequency) { this->frequency_ = frequency; }

        protected:
            uint8_t pin_;
            float sensitivity_{1.0};
            uint16_t frequency_{60};
            uint32_t period_us_;

            int get_zero_point();
            float get_rms_voltage(uint8_t loop_count);
        };

    }  // namespace zmpt101b
}  // namespace esphome
