#include "iostm8s105c6.h"
#define DS PD_ODR,PD_IDR 

void delay(long n)
{
    for(long i = 0;i < n; i++)
    {
        asm("NOP");
    }
}

void uart_tx_byte(unsigned char ReceivedData)
{
    while(!UART2_SR_TXE)
    {
        asm("NOP");
    }
    
    UART2_DR = ReceivedData;
}

unsigned char uart_rx_byte()
{
    while(!UART2_SR_RXNE)
    {
        asm("NOP");
    }
    
    unsigned char ActualData = UART2_DR;
    return ActualData;
}

#ifdef UART_INT_ENABLE

    /// Method uart_rx_interrupt.
    /// UART Receive Interrupt Handler.
    #pragma vector=UART2_SR_RXNE_vector 
    __interrupt void uart_rx_interrupt(void)
    {
      unsigned char data;
      data = UART2_DR;
      
      //Sends the received byte back.
      uart_tx_byte(data);
    }
    
#endif

void main()
{
    uart2_init(UART_BAUD_RATE, F_MASTER);
    
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

/// Method uart2_init.
/// Initialization of UART2 STM8S
void uart2_init(unsigned long baud_rate, unsigned long f_master){
  
  // BRR register value.
  unsigned long brr;
  
  //Configure TX on the output, and RX on the input.
  PD_DDR_bit.DDR5 = 1;  //TX
  PD_DDR_bit.DDR6 = 0;  //RX
  
  //RX - ��������� ����
  PD_CR1_bit.C16 = 0;
  //��������� ������� ���������� ��� RX
  PD_CR2_bit.C26 = 0;
  //��������� �������� ��������
  brr = f_master/baud_rate;
    
  UART2_BRR2 = brr & 0x000F;
  UART2_BRR2 |= brr >> 12;
  UART2_BRR1 = (brr >> 4) & 0x00FF;
  //�������� ���������
  UART2_CR1_PIEN = 0;
  //�������� �������� ��������
  UART2_CR1_PCEN = 0;
  //8-������ �����
  UART2_CR1_M = 0;
  //�������� UART
  UART2_CR1_UART0 = 0;
  
  //��������� ���������� �� ����������� ��������. ��������
  UART2_CR2_TIEN = 0;
  //��������� ���������� �� ���������� ��������
  UART2_CR2_TCIEN = 0;
  //��������� ���������� �� ���������� ��������� ��������
  #ifdef UART_INT_ENABLE  
    UART2_CR2_RIEN = 1;
  #else
    UART2_CR2_RIEN = 0;
  #endif
  //��������� ���������� �� ������������ �����
  UART2_CR2_ILIEN = 0;
  //���������� ��������
  UART2_CR2_TEN = 1;
  //�������� ��������
  UART2_CR2_REN = 1;
  //�� �������� break-������
  UART2_CR2_SBK = 0;
  //���� ����-���
  UART2_CR3_STOP = 0;
}