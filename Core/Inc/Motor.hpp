//
// Created by a2005 on 2025/10/3.
//

#ifndef ABOARD_1_MOTOR_HPP
#define ABOARD_1_MOTOR_HPP

#include <cstdint>

inline float linearMapping(float value, float in_min, float in_max, float out_min, float out_max) {
  return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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
  M3508_Motor() = delete;

  explicit M3508_Motor(float ratio) : ratio_(ratio) {
  }

  void parse_CANMsg_callback(const uint8_t rx_data[8]) {
    // Get Current EncoderAngle, Map to [0, 360)
    ecd_angle_ = linearMapping(static_cast<float>(rx_data[0] << 8 | rx_data[1]),
                               0, 8191, 0, 360);
    // Get DeltaEncoderAngle, range [-180, 180)
    delta_ecd_angle_ = ecd_angle_ - last_ecd_angle_;
    if (delta_ecd_angle_ < -180) delta_ecd_angle_ += 360;
    if (delta_ecd_angle_ > 180) delta_ecd_angle_ -= 360;
    // Accumulate Actual Angle
    last_ecd_angle_ = ecd_angle_;
    delta_angle_ = delta_ecd_angle_ / ratio_;
    angle_ += delta_angle_;
    if (angle_ > 360) angle_ -= 360;
    if (angle_ < 0) angle_ += 360;

    // Get other information
    rotate_speed_ = static_cast<float>(rx_data[2] << 8 | rx_data[3]);
    current_ = linearMapping(static_cast<float>(rx_data[4] << 8 | rx_data[5]),
                             -16384, 16384, -20, 20);
    temp_ = static_cast<float>(rx_data[6]);
  }
};


class PID {
private:
  float Kp_, Ki_, Kd_;
  float prev_error_, sum_error_;

public:
  PID() = delete;

  PID(const float Kp, const float Ki, const float Kd) : Kp_(Kp), Ki_(Ki), Kd_(Kd) {
    prev_error_ = 0;
    sum_error_ = 0;
  }

  float calculate(float error) {
    float res = Kp_ * error + Ki_ * sum_error_ + Kd_ * (error - prev_error_);
    sum_error_ += error;
    prev_error_ = error;
    return res;
  }
};

#endif //ABOARD_1_MOTOR_HPP
