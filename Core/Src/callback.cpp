//
// Created by JJC on 2025/10/2.
//
#include <main.h>
#include <tim.h>
#include <usart.h>
#include <cmath>
extern uint8_t RX_buffer[20];

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
  if (htim == &htim1) {
    static uint32_t cnt = 0;
    uint32_t arr_value = __HAL_TIM_GET_AUTORELOAD(htim) + 1;
    __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, ++cnt % arr_value);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart7) {
    if (RX_buffer[0] == 'R')  HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
    else if (RX_buffer[0] == 'M') HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
  }
  HAL_UART_Receive_IT(huart, RX_buffer, 1);
}
