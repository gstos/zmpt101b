#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
    namespace zmpt101b {

        class ZMPT101BSensor : public sensor::Sensor, public PollingComponent {
        public:
            ZMPT101BSensor(uint8_t pin, float sensitivity = 1.0, uint16_t frequency = 60)
                : pin_(pin), sensitivity_(sensitivity), frequency_(frequency) {}

            void setup() override {
                pinMode(this->pin_, INPUT);
                this->period_us_ = 1000000UL / frequency_;
            }

            void update() override {
                float rms = get_rms_voltage(5);  // 5 measurement loops
                this->publish_state(rms);
            }

            void set_sensitivity(float s) { sensitivity_ = s; }
            void set_frequency(uint16_t f) { frequency_ = f; }

        protected:
            uint8_t pin_;
            float sensitivity_;
            uint16_t frequency_;
            uint32_t period_us_;

            int get_zero_point() {
                uint32_t sum = 0;
                uint32_t count = 0;
                uint32_t start = micros();
                while (micros() - start < period_us_) {
                    sum += analogRead(pin_);
                    count++;
                }
                return count ? (sum / count) : 2048;
            }

            float get_rms_voltage(uint8_t loop_count) {
                float total = 0;
                for (uint8_t i = 0; i < loop_count; i++) {
                    int zero = get_zero_point();
                    uint32_t sum_sq = 0;
                    uint32_t count = 0;
                    uint32_t start = micros();
                    while (micros() - start < period_us_) {
                        int32_t val = analogRead(pin_) - zero;
                        sum_sq += val * val;
                        count++;
                    }
                    float vrms = sqrtf((float)sum_sq / count) / 4096.0f * 3.3f * sensitivity_;  // ADC_SCALE=4096, VREF=3.3
                    total += vrms;
                }
                return total / loop_count;
            }
        };

    }  // namespace zmpt101b
}  // namespace esphome