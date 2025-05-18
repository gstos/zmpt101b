#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include <Arduino.h>

namespace esphome {
    namespace zmpt101b {

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

        class ZMPT101BSensor : public sensor::Sensor, public PollingComponent {
        public:
            ZMPT101BSensor(uint8_t pin, float sensitivity = 509.0f, uint16_t frequency = 50);

            void setup() override;
            void update() override;
            float get_setup_priority() const override { return setup_priority::HARDWARE; }

            void set_sensitivity(float s) { sensitivity_ = s; }
            void set_frequency(uint16_t f) { frequency_ = f; }

        protected:
            uint8_t pin_;
            float sensitivity_;
            uint16_t frequency_;
            uint32_t period_us_;

            int get_zero_point();
            float get_rms_voltage(uint8_t loop_count = 1);
        };

    }  // namespace zmpt101b
}  // namespace esphome
