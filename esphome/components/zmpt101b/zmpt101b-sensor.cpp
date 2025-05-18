#include "zmpt101b_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
    namespace zmpt101b {

        static const char *TAG = "zmpt101b";

        ZMPT101BSensor::ZMPT101BSensor(uint8_t pin) : pin_(pin) {}

        void ZMPT101BSensor::setup() {
            pinMode(this->pin_, INPUT);
            this->period_us_ = 1000000UL / this->frequency_;
        }

        void ZMPT101BSensor::update() {
            float rms = get_rms_voltage(5);  // average over 5 AC cycles
            this->publish_state(rms);
        }

        void ZMPT101BSensor::dump_config() {
            ESP_LOGCONFIG(TAG, "ZMPT101B Sensor:");
            ESP_LOGCONFIG(TAG, "  Pin: GPIO%d", this->pin_);
            ESP_LOGCONFIG(TAG, "  Frequency: %d Hz", this->frequency_);
            ESP_LOGCONFIG(TAG, "  Sensitivity: %.3f", this->sensitivity_);
        }

        int ZMPT101BSensor::get_zero_point() {
            uint32_t sum = 0;
            uint32_t count = 0;
            uint32_t start = micros();
            while (micros() - start < period_us_) {
                sum += analogRead(pin_);
                count++;
            }
            return count ? (sum / count) : 2048;
        }

        float ZMPT101BSensor::get_rms_voltage(uint8_t loop_count) {
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
                float vrms = sqrtf((float)sum_sq / count) / 4096.0f * 3.3f * sensitivity_;
                total += vrms;
            }
            return total / loop_count;
        }

    }  // namespace zmpt101b
}  // namespace esphome