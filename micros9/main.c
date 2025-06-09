#include <msp430.h>
#include "uart.h"
#include <stdlib.h>

#define RED mode = 0;
#define GREEN mode = 1;
#define BLUE mode = 2;

volatile char contador = 0;

char red[5] = "128\n";
char blue[5] = "128\n";
char green[5] = "128\n";

int mode = 3;

int main(void){

   WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer
   BCSCTL1 = CALBC1_16MHZ;
   DCOCTL = CALDCO_16MHZ ;

   TA0CTL = TASSEL_2 + MC_1 + ID_3;
   TA0CCR0 =32640;
   TA0CCTL1 = OUTMOD_7;
   TA0CCR1 = 32640; //verde
   P1DIR |= BIT6;
   P1SEL |= BIT6;
   P1SEL2 &= ~BIT6;

   TA1CTL = TASSEL_2 + MC_1 + ID_3;
   TA1CCR0 =32640;
   TA1CCTL1 = OUTMOD_7;
   TA1CCR1 = 32640; //rojo
   P2DIR |= BIT1;
   P2SEL |= BIT1;
   P2SEL2 &= ~BIT1;

   TA1CCTL2 = OUTMOD_7;
   TA1CCR2 = 32640; //azul
   P2DIR |= BIT5;
   P2SEL |= BIT5;
   P2SEL2 &= ~BIT5;

   P1SEL |= BIT1 + BIT2;
   P1SEL2 |= BIT1 + BIT2;
   P1DIR &= ~BIT1;
   P1DIR |= BIT2;
   UCA0CTL1 |= UCSSEL_2;

    UCA0BR0 = 0x82;                       // 16MHz 9600
    UCA0BR1 = 0x06;                       // 16MHz 9600
    UCA0MCTL = UCBRS2+UCBRS1;             // Modulation UCBRSx = 6

   UCA0CTL1 &= ~UCSWRST;
   IE2 |= UCA0RXIE;
 
   __bis_SR_register(GIE);

   print(red);
  while(1){
    
  }

}


#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    if(UCA0RXBUF == 'R'){
        RED;
        return;
    }
    if(UCA0RXBUF == 'G'){
        GREEN;
        return;
    }
    if(UCA0RXBUF == 'B'){
        BLUE;
        return;
    }
    if(UCA0RXBUF == '\n'){
        mode = 3;
        contador = 0;
        TA0CCR1 = (atoi(green)) * 128;
        TA1CCR1 = (atoi(red)) * 128;
        TA1CCR2 = (atoi(blue)) * 128;
        return;
    }

    switch(mode){
        case 0:
            red[contador] = UCA0RXBUF;
            contador++;
            break; 
        case 1:
            green[contador] = UCA0RXBUF;
            contador++;
            break;
        case 2:
            blue[contador] = UCA0RXBUF;
            contador++;
            break;
        default:
            break;
    }
    

}