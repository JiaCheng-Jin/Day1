//
// Created by JJC on 2025/10/2.
//
#include <main.h>
#include <tim.h>
#include <cmath>
// Override __weak
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin) {
    case KEY_Pin: {
      break;
    }

    default:{}
  }
}

// Override __weak
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  uint32_t arr_value = __HAL_TIM_GET_AUTORELOAD(htim) + 1;
  uint32_t brightness = arr_value * sinf(4 * HAL_GetTick() / 1000.0f) - 1;
  __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, brightness);
}
