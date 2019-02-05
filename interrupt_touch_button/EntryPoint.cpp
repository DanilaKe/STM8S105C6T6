#include "iostm8.h" 
#include "stdint.h"
#include "intrinsics.h"

void main()
{
    led_init();
    asm("RIM"); //  Globally enable interrupt handling.
    
    //  Infinitely flashing diode.
    while (true)
    {
      PD_ODR_ODR0 = !PD_ODR_ODR0;
      wait(10000);
    }
}

void led_init()
{
    __disable_interrupt();
    //  Init D Port.
    PD_ODR = 0b00000000; //  All pins are turned off.
    PD_DDR = 0b11111111; //  All pins D are outputs.
    PD_CR1 = 0b11111111; //  Push-Pull outputs.
    PD_CR2 = 0b11111111; //  Output speeds up to 10 MHz.
    //  Init C Port.
    PC_DDR = 0b00000000; //  All pins C are inputs.
    PC_CR1 = 0b00000000; //  All pins D is floating input
    PC_CR2 = 0b11111111; //  Input speeds up to 10 MHz.
    // Init external interrupts.
    EXTI_CR1_PCIS = 0b00000010; //  Interrupt on falling edge only.
    EXTI_CR2_TLIS = 0b00000000; //  Falling edge only.
    __enable_interrupt();
}

#pragma vector = 7
    /// Method EXTI_PORTC_IRQHandler.
    /// PORTC Interrupt Handler.
    __interrupt void EXTI_PORTC_IRQHandler(void)
    {
        int NumberOfCycles = 5;
        int WaitingTime = 45000;
    
        for(int i = 0; i < NumberOfCycles; i++)
        {
            PD_ODR_ODR0 = !PD_ODR_ODR0;
            wait(WaitingTime);
        }
    }

/// Method wait.
/// Necessary for waiting.
void wait(long n)
{
    while (n >= 0)
    {
        --n;
    }
}