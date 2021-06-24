#include <Arduino.h>
#include <dma.h>
#include <usart.h>

#define RS485_RE PG3

static uint8_t tx_buf[] = "这是一个串口中断接收回显实验\n";

void setup(void)
{
    pinMode(RS485_RE, OUTPUT);
    digitalWrite(RS485_RE, HIGH);
    // Serial6.begin(9600);
    MX_DMA_Init();
    MX_USART2_UART_Init();
    HAL_UART_Transmit_DMA(&huart2,tx_buf,sizeof(tx_buf));           //串口DMA发送一帧数据
}

void loop(void)
{
    // Serial6.println("123456");
    // delay(1000);
}