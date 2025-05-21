#include "intrinsics.h"
#include <msp430.h> 
//#include "main.h"
#define Boton_1 (P2IN & BIT0)
#define Boton_2 (P2IN & BIT1)
#define Boton_3 (P2IN & BIT2)

int contador = 0;
char e = 20, i = 0,  time = 1; 
int main(void){

    WDTCTL = (WDTPW + WDTHOLD);   // stop watchdog timer

    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    P1DIR = 0xFF;
    P1SEL = 0x00;
    P1SEL2 = 0x00;

    P2DIR &= ~(BIT0 + BIT1 + BIT2);
    P2SEL &= ~(BIT0 + BIT1 + BIT2);
    P2SEL2 &= ~(BIT0 + BIT1 + BIT2);

    P2REN |= (BIT0 + BIT1 + BIT2);
    P2OUT &= ~(BIT0 + BIT1 + BIT2);

    P2IE |= (BIT0 + BIT1 + BIT2);
    P2IES &= ~(BIT0 + BIT1 + BIT2); 
    P2IFG = 0x00; //registro

    P1OUT = 0x00;

    __bis_SR_register(GIE);

    P1OUT = 0x00;

    while (1){
        if(i == 0){
            if(contador > 20) contador = 0;
            contador += 2;
            P1OUT = contador;
        }
        if(i == 1){
            if(contador < 0) contador = 20;
            contador -= 2;
            P1OUT = contador; 
        }
        __delay_cycles(4000000);
        if(time == 2)
            __delay_cycles(4000000);
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void Boton(void){
    if(Boton_1 == BIT0){
         e = (e == 10 )? 20 : 10;
         P1OUT = e;
         __delay_cycles(10000000);
    }
    if(Boton_2 == BIT1){
        i ^= 1;
    }
    if(Boton_3 == BIT2){
        time = (time == 1)? 2 : 1; 
    }
    __delay_cycles(50000);
    P2IFG = 0x00;
}
 