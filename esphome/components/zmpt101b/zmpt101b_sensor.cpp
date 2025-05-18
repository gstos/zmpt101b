#include "zmpt101b_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
    namespace zmpt101b {

        static const char *TAG = "zmpt101b";

        ZMPT101BSensor::ZMPT101BSensor(uint8_t pin, float sensitivity, uint16_t frequency)
            : pin_(pin), sensitivity_(sensitivity), frequency_(frequency) {}

        void ZMPT101BSensor::setup() {
            pinMode(this->pin_, INPUT);
            this->period_us_ = 1000000UL / frequency_;
        }

        void ZMPT101BSensor::update() {
            float Vrms = this->get_rms_voltage();
            ESP_LOGD(TAG, "Pin %d RMS voltage: %.2f V", pin_, Vrms);
            this->publish_state(Vrms);
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
            double readingVoltage = 0.0;

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

                readingVoltage += sqrt((float)sum_sq / count) / ADC_SCALE * VREF * sensitivity_;
            }

            return readingVoltage / loop_count;
        }

    }  // namespace zmpt101b
}  // namespace esphome