//
// Created by JJC on 2025/10/2.
//
#include <main.h>
#include <tim.h>
#include <usart.h>
#include <cstring>
#include <can.h>

uint32_t can_tx_mailbox;
extern uint8_t RX_buffer[20], TX_buffer[20];

uint8_t rx_data[8];
uint8_t tx_data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00};
CAN_TxHeaderTypeDef tx_header = {
  .StdId = 0x200,
  .ExtId = 0x000,
  .IDE = CAN_ID_STD,
  .RTR = CAN_RTR_DATA,
  .DLC = 8,
  .TransmitGlobalTime = DISABLE
};
CAN_RxHeaderTypeDef rx_header;

// Override __weak
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
    case KEY_Pin: {
      break;
    }

    default: {
    }
  }
}

// Override __weak
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim1) {
    static uint32_t cnt = 0;
    uint32_t arr_value = __HAL_TIM_GET_AUTORELOAD(htim) + 1;
    __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, ++cnt % arr_value);
  }
  if (htim == &htim6) {
    HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data, &can_tx_mailbox);
  }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  if (huart == &huart7 && huart->RxEventType != HAL_UART_RXEVENT_HT) {
    memcpy(TX_buffer, RX_buffer, Size);
    HAL_UART_Transmit_DMA(huart, TX_buffer, Size);
    HAL_UARTEx_ReceiveToIdle_DMA(huart, RX_buffer, 20);
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  if (hcan == &hcan1) {
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);
  }
}
