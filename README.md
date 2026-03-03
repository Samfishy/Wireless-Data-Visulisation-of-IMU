# Wireless-Data-Visulisation-of-IMU
I built a real-time telemetry receiver that wirelessly captures Roll, Pitch &amp; Yaw from a remote IMU node and renders them live on a 1.8" TFT display — no RTOS, no HAL blocking, just clean interrupt-driven embedded firmware.

NRF24L01+  →  2.4GHz wireless with ACK payload, fully interrupt-driven (EXTI on IRQ pin)
MPU9250    →  9-axis IMU on the transmitter node,sending float[3] {pitch, yaw, roll} packed as a 12-byte NRF24 payload
LVGL 9    →  3 animated progress bars + 3 float textarea readouts, UI built in EEZ Studio
SPI DMA    →  Non-blocking ST7735 flush via HAL_SPI_Transmit_DMA(); CPU is free during every display transfer
TIM3      →  Schedules lv_timer_handler() at exactly the interval LVGL requests

## Hardware
| Component         | Detail                        |
|-------------------|-------------------------------|
| MCU               | STM32F411RE (Nucleo-64)       |
| Display           | 1.8" ST7735 TFT (160x128)     |
| Wireless          | NRF24L01+ (SPI2)              |
| IMU (Transmitter) | MPU9250 (on remote node)      |
| IDE               | STM32CubeIDE                  |
| GUI Framework     | LVGL 9 + EEZ Studio           |

## Pin Mapping
| Signal         | STM32 Pin   |
|----------------|-------------|
| ST7735 SPI     | SPI1        |
| ST7735 CS      | PA9         |
| ST7735 DC/RST  | PA10 / PB10 |
| NRF24 SPI      | SPI2        |
| NRF24 CSN      | PB1         |
| NRF24 CE       | PB2         |
| NRF24 IRQ      | PB12        |

## Architecture

NRF24 IRQ (EXTI PB12)
        |
        v
   irq flag set
        |
        v
  main loop: nrf24_receive()
        |
        v
  memcpy -> attitude[3] (pitch, yaw, roll as floats)
        |
        v
  lv_bar_set_value() + lv_textarea_set_text()
        |
        v
  lv_timer_handler() -> TIM3 schedules next render
        |
        v
  my_flush_cb() -> ST7735_DrawImage_DMA() -> HAL_SPI_TxCpltCallback
                                                    |
                                            lv_disp
