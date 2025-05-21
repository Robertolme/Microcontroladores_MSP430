#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


    P1DIR = P1DIR | 0x03;    
    P1SEL0 = P1SEL0 & 0xFC;  
    P1SEL1 = P1SEL1 & 0xFC;  

    P2DIR &= ~(BIT3+BIT7);   
    P2SEL0 &= ~(BIT3+BIT7);  
    P2SEL1 &= ~(BIT3+BIT7); 

    P2REN |= BIT3+BIT7;      
    P2OUT |= BIT3+BIT7;      

    PM5CTL0 &= ~LOCKLPM5;    
    P1OUT &= ~(BIT0+BIT1);

    while (1){

        if((P2IN & BIT3) == 0) {
            if ((P1OUT & BIT0) == BIT0)
                P1OUT &= ~BIT0;
            else
                P1OUT |= BIT0;

            while((P2IN & BIT3) == 0){}
        }


        if((P2IN & BIT7) == 0) {
            if ((P1OUT & BIT1) == BIT1)
                P1OUT &= ~BIT1;
            else
                P1OUT |= BIT1;
            while((P2IN & BIT7) == 0){} 
        }

    }
}