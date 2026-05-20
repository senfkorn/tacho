#pragma once

#include "esphome.h"
#include "driver/pulse_cnt.h"

namespace esphome {
namespace pcnt_encoder {

class PCNTEncoder : public PollingComponent {
 public:
  PCNTEncoder(gpio_num_t pin_a, gpio_num_t pin_b, float meters_per_pulse);

  void set_speed_sensor(Sensor *s) { speed_sensor_ = s; }
  void set_distance_sensor(Sensor *s) { distance_sensor_ = s; }

  void setup() override;
  void update() override;

 private:
  gpio_num_t pin_a_;
  gpio_num_t pin_b_;
  float meters_per_pulse_;

  float total_distance_ = 0;

  Sensor *speed_sensor_ = nullptr;
  Sensor *distance_sensor_ = nullptr;

  pcnt_unit_handle_t unit_;
  pcnt_channel_handle_t channel_;
};

}  // namespace pcnt_encoder
}  // namespace esphome
