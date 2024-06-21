/**
 * @file    main.c
 * @brief   Pedometro with EFM32GG_STK3700
 * @version 1.0
 *
 * @note    Just blinks the LEDs of the STK3700
 *
 * @note    LEDs are on pins 2 and 3 of GPIO Port E
 *
 * @note    It uses a primitive delay mechanism. Do not use it.
 *
 * @author  Mateus Sartorio
 * @date    01/09/2018
 */
#include <stdint.h>

#include "../include/button.h"
#include "../include/led.h"
#include "clock_efm32gg.h"
#include "em_device.h"

#define true 1
#define false 0

void button_callback(uint32_t mask) {
    if(Button_ReadReleased()) {
        LED_Toggle(LED1);
        // if(BUTTON1 & mask) {
        //     LED_Toggle(LED1);
        // }

        // if(BUTTON2 & mask) {
        //     LED_Toggle(LED2);
        // }
    }
}

void SysTick_Handler(void) { Button_Processing(); }

int main(void) {
    LED_Init(LED1 | LED2);

    Button_Init(BUTTON1 | BUTTON2);
    Button_SetCallback(button_callback);

    __enable_irq();

    LED_Write(LED1 | LED2, 0);

    while(true) {
        uint32_t button = Button_ReadReleased();

        if(button & BUTTON1) {
            LED_Toggle(LED1);
        }

        if(button & BUTTON2) {
            LED_Toggle(LED2);
        }
    }
}