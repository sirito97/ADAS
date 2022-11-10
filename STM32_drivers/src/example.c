// Libraries
#include "LIBRARY/BIT_MATH.h"
#include "LIBRARY/STD_TYPES.h"
// Interfaces of MCAL
#include "GPIO/GPIO_interface.h"
#include "RCC/RCC_interface.h"
#include "SYSTICK/SYSTICK_interface.h"
#include "EXTI/EXTI_interface.h"
#include "NVIC/NVIC_interface.h"
#include "USART/USART_interface.h"
#include "TIMER/TIMER_interface.h"
#include "TIMER/TIMER_private.h"

int main(void) {
    // System clock initialize to high speed external clock
    RCC_Init();
    // Enable port GPIOA clock on APB2 bus
    RCC_PeripheralClockEnable(RCC_APB2, RCC_GPIOA);
    // Enable USART 1 clock
    RCC_PeripheralClockEnable(RCC_APB2, RCC_USART1);

    RCC_PeripheralClockEnable(RCC_APB1, RCC_TIM2);

    RCC_PeripheralClockEnable(RCC_APB2, RCC_AFIO);

    // GPIO_SetPinMode(GPIO_PORTA, USART_TxPin , GPIO_OUTPUT_ALT_OD_2MHZ);
    GPIO_SetPinMode(GPIO_PORTA, PIN0, GPIO_OUTPUT_ALT_OD_2MHZ);
    // GPIO_SetPinMode(GPIO_PORTA, PIN1, GPIO_OUTPUT_GP_PP_2MHZ);
    GPIO_SetPinValue(GPIO_PORTA, PIN0, 0);
    //  GPIO_SetPinValue(GPIO_PORTA, PIN1, 0);
    // Interrupt line 0 (PIN A0)
    // EXTI_void_ControlInterruptLine(0, EXTI_LINE_ENABLE);
    // Interrupt trigger type
    //EXTI_void_SelectLineTriggerType(0, EXTI_LineTrigger_RISING);
    // Enable EXTI0 interrupt
    //NVIC_voidEnableInterrupt(6);
    // USART_SetBaudRate(BAUD_RATE_9600,USART_1);
    //USART_EnableUSART(USART_1);
    //u8 a = 0;
    u32 CH1_DC = 0;
    TIM2_Init(TIM_CHANNEL_1);
    while (1){
        while(CH1_DC<65535){
            TIM2_PWMSet(CH1_DC,TIM_CHANNEL_1);
            CH1_DC += 2000;
            TICK_Delay(1000);
        }
        while(CH1_DC > 0){
            TIM2_PWMSet(CH1_DC,TIM_CHANNEL_1);
            CH1_DC -= 2000;
            TICK_Delay(1000);
        }
        // GPIO_SetPinValue(GPIO_PORTA, PIN1, 1);
        // TICK_Delay(1000);
        // USART_SendByte(USART_1, a);
        // a++;
        // GPIO_SetPinValue(GPIO_PORTA, PIN1, 0);
        // TICK_Delay(1000);
        // USART_SendByte(USART_1, a);
        // a++;
    }
    return 0;
}
// Comment this because it is defined in ultrasonic program.c
// void EXTI0_IRQHandler(void)
// {
//     static u8 x = 1;
//     GPIO_SetPinValue(GPIO_PORTA, PIN2, x);
//     // NVIC_voidSetPendingFlag(7);
//     // while(1);
//     x = 1 - x;
// }