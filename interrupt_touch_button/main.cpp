#include "iostm8.h" 
#include "stdint.h"
#include "intrinsics.h"
void wait(long n)
{
  for(long i = 0;i<n;i++)
    asm("NOP");
}
#pragma vector = 7
__interrupt void EXTI_PORTC_IRQHandler(void)
{
  for(int i = 0;i<5;i++)
  {
    PD_ODR_ODR0 = !PD_ODR_ODR0;
    wait(45000);
  }
}
void main()
{
    __disable_interrupt();
    PD_ODR = 0;
    PD_DDR = 0xff;
    PD_CR1 = 0xff; 
    PD_CR2 = 0xff; 
    PC_DDR = 0x00;
    PC_CR1 = 0x00;
    PC_CR2 = 0xff;
    EXTI_CR1_PCIS = 2;   
    EXTI_CR2_TLIS = 0; 
    __enable_interrupt();
    asm("RIM");
    while (1)
    {
      PD_ODR_ODR0 = !PD_ODR_ODR0;
      wait(10000);
    }
}