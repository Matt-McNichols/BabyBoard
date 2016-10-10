#include <stdio.h>
#include "msp430g2553.h"

/*
In asynchronous mode, the USCI_Ax modules connect the MSP430 to an external
system via two external pins, UCAxRXD and UCAxTXD. UART mode is selected when
the UCSYNC bit is cleared.

The recommended USCI initialization/re-configuration process is:
1. Set UCSWRST (BIS.B #UCSWRST,&UCAxCTL1)
2. Initialize all USCI registers with UCSWRST = 1 (including UCAxCTL1)
3. Configure ports.
4. Clear UCSWRST via software (BIC.B #UCSWRST,&UCAxCTL1)
5. Enable interrupts (optional) via UCAxRXIE and/or UCAxTXIE
Control Registers:
    UCAxCTL0:
        B7- parity enable (1 for enabled)
        B6- parity selected
        b5- MSB first select (1 for msb first)
        B4- char lenght select (0 for 8 bit)
        B3- Stop bit select (1 for two stop bits)
        B(2,1)- USCI Mode (00 for UART)
        B0- sync mode enable (0 for asynchronous)
    UCAXCTL1:
        B(7-6)- clock source select
        B5- erroneous char interrupt en
        B4- break char interrupt en
        B3- sleep mode
        B2- TX address (0 for data, 1 for address)
        B1- transmit break (only if in auto baud rate mode)
        B0- Software reset enabled (1 for reset state)

use usci a0
*/
#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1

void init_uart(char baud){
   DCOCTL = 0; // Select lowest DCOx and MODx settings
   BCSCTL1 = CALBC1_1MHZ; // Set DCO
   DCOCTL = CALDCO_1MHZ;
   //P2DIR |= 0xFF; // All P2.x outputs
   //P2OUT &= 0x00; // All P2.x reset
   //P1SEL |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
   //P1SEL2 |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
   P1DIR |= RXLED + TXLED;
   P1OUT &= 0x00;
   // SMCLK
   UCA0CTL1 |= UCSSEL_2; 
   // baud rate 
   UCA0BR0 = 0x08; // 1MHz 115200
   UCA0BR1 = 0x00; // 1MHz 115200
   UCA0MCTL = UCBRS2 + UCBRS0; // Modulation UCBRSx = 5
   UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
   UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
  /*
   __bis_SR_register(CPUOFF + GIE); // Enter LPM0 w/ int until Byte RXed
   while (1)
   { }
  */
}


// putch: Send an unsigned char via UART. 
void putch(unsigned char c);


// put_str: Send each element of a null-terminated array of unsigned chars via UART.  Do not send the final null-termination character.
void put_str(unsigned char* c);
    // just call putch until null char

int uart_rx(char block);
