#include "iostm8s105c6.h"
#define   DS      PD_ODR,PD_IDR 


void delay(long n)
{
  for(long i = 0;i < n; i++)
  asm("NOP");
}
void uart_tx_byte(unsigned char data){
    while(!UART2_SR_TXE);
    UART2_DR = data;
}


unsigned char uart_rx_byte(){ 
    unsigned char data;
    while(!UART2_SR_RXNE);
    data = UART2_DR;
    return data;
}


#ifdef UART_INT_ENABLE  
//******************************************************************************
// Обработчик прерываний по приему UART
//******************************************************************************
#pragma vector=UART2_SR_RXNE_vector 
__interrupt void uart_rx_interrupt(void){
  unsigned char data;
  
  data = UART2_DR;
  
  //Отсылаем принятый байт обратно
  uart_tx_byte(data);
}
#endif


void main()
{
  unsigned long brr;
  PD_DDR_bit.DDR5 = 1;  
  PD_DDR_bit.DDR6 = 0;  
  PD_CR1_bit.C16 = 0;
  PD_CR2_bit.C26 = 0;
  brr = 9600*2000000;
  UART2_BRR2 = brr & 0x000F;
  UART2_BRR2 |= brr >> 12;
  UART2_BRR1 = (brr >> 4) & 0x00FF;
  UART2_CR1_PIEN = 0;
  UART2_CR1_PCEN = 0;
  UART2_CR1_M = 0;
  UART2_CR1_UART0 = 0;
  UART2_CR2_TIEN = 0;
  UART2_CR2_TCIEN = 0;
#ifdef UART_INT_ENABLE  
  UART2_CR2_RIEN = 1;
#else
  UART2_CR2_RIEN = 0;
#endif  
  UART2_CR2_ILIEN = 0;
  UART2_CR2_TEN = 1;
  UART2_CR2_REN = 1;
  UART2_CR2_SBK = 0;
  UART2_CR3_STOP = 0;
  uart_tx_byte(0xcc);
  uart_tx_byte(0x44);
  uart_tx_byte(0xcc);
  uart_tx_byte(0xBE);
  char data[2];
  int temp;
  while(1)
  {
    data[0] = uart_rx_byte();
    data[1] = uart_rx_byte();
    temp = data[1];
    temp = temp<<8;
    temp |= data[0];
    temp *= 0.0625;
  }
}

