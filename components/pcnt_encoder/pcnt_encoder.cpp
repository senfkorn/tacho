#include "pcnt_encoder.h"

namespace esphome {
namespace pcnt_encoder {

PCNTEncoder::PCNTEncoder(gpio_num_t pin_a, gpio_num_t pin_b, float meters_per_pulse)
    : PollingComponent(200),
      pin_a_(pin_a),
      pin_b_(pin_b),
      meters_per_pulse_(meters_per_pulse) {}

void PCNTEncoder::setup() {
  pcnt_unit_config_t unit_config = {};
  unit_config.high_limit = 100000;
  unit_config.low_limit = -100000;

  pcnt_new_unit(&unit_config, &unit_);

  pcnt_chan_config_t chan = {};
  chan.edge_gpio_num = pin_a_;
  chan.level_gpio_num = pin_b_;

  pcnt_channel_new(unit_, &chan, &channel_);

  pcnt_chan_set_edge_action(
      channel_,
      PCNT_CHANNEL_EDGE_ACTION_INCREASE,
      PCNT_CHANNEL_EDGE_ACTION_DECREASE);

  pcnt_unit_enable(unit_);
  pcnt_unit_start(unit_);
}

void PCNTEncoder::update() {
  int count = 0;
  pcnt_unit_get_count(unit_, &count);
  pcnt_unit_clear_count(unit_);

  float delta_m = count * meters_per_pulse_;
  total_distance_ += delta_m;

  float seconds = this->get_update_interval() / 1000.0f;
  float speed_m_min = (delta_m / seconds) * 60.0f;

  if (speed_sensor != nullptr)
    speed_sensor->publish_state(speed_m_min);

  if (distance_sensor != nullptr)
    distance_sensor->publish_state(total_distance_);
}

}  // namespace pcnt_encoder
}  // namespace esphome
