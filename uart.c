#include”msp430g2553.h”
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
*/

void init_uart(char baud);
