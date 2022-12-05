#include "LIBRARY/BIT_MATH.h"
#include "LIBRARY/STD_TYPES.h"

#include "GPIO/GPIO_interface.h"
#include "RCC/RCC_interface.h"
#include "SYSTICK/SYSTICK_interface.h"
#include "USART/USART_interface.h"
#include "TIMER/TIMER_interface.h"
#include "EXTI/EXTI_interface.h"
#include "NVIC/NVIC_interface.h"

#include "H_BRIDGE/H_BRIDGE_interface.h"
#include "ULTRASONIC/UltraSonic_interface.h"

u16 Ticks = 0;
u32 Distance;

int main(void)
{
    RCC_Init(); // system internal clock
    RCC_PeripheralClockEnable(RCC_APB2, RCC_GPIOA);
    RCC_PeripheralClockEnable(RCC_APB2, RCC_GPIOC);
    RCC_PeripheralClockEnable(RCC_APB1, RCC_TIM3); // RCC for Timer 3
    USART_Start(BAUD_RATE_115200, USART_1);
    GPIO_SetPinMode(GPIO_PORTC, PIN13, GPIO_OUTPUT_GP_PP_10MHZ); // built-in LED
    GPIO_SetPinValue(GPIO_PORTC, PIN13, GPIO_HIGH);

    GPIO_SetPinMode(GPIO_PORTA, 0, GPIO_INPUT_PULLING); // pin A0 (interrupt) as input
    GPIO_SetPinValue(GPIO_PORTA, 0, GPIO_LOW);

    EXTI_void_ControlInterruptLine(0, EXTI_LINE_ENABLE);         // enable external interrupt line 0 (EXTI0) (A0)
    EXTI_void_SelectLineTriggerType(0, EXTI_LineTrigger_RISING); // select trigger type (rising edge)
    NVIC_voidEnableInterrupt(6);                                 // enable interrupt 6 (EXTI0)

    TIMER_Init(TIMER_NUM_3, 1);
    UltrSonic_INIT();
    GPIO_SetPinValue(GPIO_PORTC, PIN13, 0);
    
    while (1)
    {


        UltraSonic_Trigger();
        if (Distance < 5)
        {
            GPIO_SetPinValue(GPIO_PORTC, PIN13, 0);
        }
        else
        {
            GPIO_SetPinValue(GPIO_PORTC, PIN13, 1);
        }
        
        USART_SendByte(USART_1, Distance);
        TICK_Delay(100);
        
        // // USART_SendByte(USART_1, Ticks);
        // // TICK_Delay(100);
        // TIMER_startCounting(TIMER_NUM_3);
       
        // while(TIMER_elapsedTicks(TIMER_NUM_3)<60000){
        //     //USART_SendByte(USART_1, 0);
        // }
        // TIMER_stopCounting(TIMER_NUM_3);
        // GPIO_SetPinValue(GPIO_PORTC, PIN13, 1);
    }
    return 0;
}

void EXTI0_IRQHandler(void)
{
    static u8 f = 0;
    if (f == 0)
    {
        //TICK_StartCounting();
        TIMER_startCounting(TIMER_NUM_3);
        EXTI_void_SelectLineTriggerType(0, EXTI_LineTrigger_FALLING);
        f = 1;
    }
    else if (f == 1)
    {
        //Ticks = TICK_ElapsedTicks();
        Ticks = TIMER_elapsedTicks(TIMER_NUM_3);
        Distance = ((Ticks / 2) * 34300) / 8000000;
        //TICK_Stop();
        TIMER_stopCounting(TIMER_NUM_3);
        f = 0;
        EXTI_void_SelectLineTriggerType(0, EXTI_LineTrigger_RISING);
    }

    EXTI_void_ClearFlag(0);
}
