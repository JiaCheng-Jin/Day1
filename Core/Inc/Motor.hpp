//
// Created by a2005 on 2025/10/3.
//

#ifndef ABOARD_1_MOTOR_HPP
#define ABOARD_1_MOTOR_HPP
#include <sys/types.h>


class M3508_Motor {
private:
  const float ratio_;

  float angle_ = 0;
  float delta_angle_ = 0;
  float ecd_angle_ = 0;
  float last_ecd_angle_ = 0;
  float delta_ecd_angle_ = 0;
  float rotate_speed_ = 0;
  float current_ = 0;
  float temp_ = 0;

public:
  explicit M3508_Motor(float ratio) : ratio_(ratio) {
  }

  void parse_CANMsg_callback(const uint8_t rx_data[8]) {
    ecd_angle_ = static_cast<uint16_t>(rx_data[0] << 8 | rx_data[1]);
    delta_ecd_angle_ = ecd_angle_ - last_ecd_angle_;
    last_ecd_angle_ = ecd_angle_;

    delta_angle_ = delta_ecd_angle_ / ratio_;
    angle_ += delta_angle_;
    rotate_speed_ = static_cast<uint16_t>(rx_data[2] << 8 | rx_data[3]);
  }
};


#endif //ABOARD_1_MOTOR_HPP
