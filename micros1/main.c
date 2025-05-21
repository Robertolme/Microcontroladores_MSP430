#include <msp430.h> 



/*
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    P1DIR |= BIT0 + BIT6; //0x01, 0x40
    P1DIR &= ~BIT3;
    P1SEL0 &= ~(BIT0 + BIT6);
    P1SEL1 &= ~(BIT0 + BIT6);


    while(1){
    et:
    if( (P1IN & BIT3) == 0)// 0100 * 0
        goto et;

    P1OUT |= BIT6;
    P1OUT &= ~BIT0;
    __delay_cycles(500000);

    P1OUT &= ~BIT6;
    P1OUT |= BIT0;
    __delay_cycles(500000);

    }
}