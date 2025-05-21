#include <msp430.h> 

const unsigned char Datos[9] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};

char i = 0; 

int main(void){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    P2DIR = 0xFF;
    P2SEL = 0x00;
    P2SEL2 = 0x00;


    P1DIR &= ~(BIT3);   
    P1SEL &= ~(BIT3);  
    P1SEL2 &= ~(BIT3);  
    P1REN |= BIT3;      
    P1OUT |= BIT3;

    P2OUT = 0xFF;      

    P1IE |= BIT3; //Permisoa
    P1IES |= BIT3; //Configurar flanco de subida o bajada. 0 subida , 1 bajada 
    P1IFG = 0x00; //registro

    __bis_SR_register(GIE);

    while (1){
        P2OUT = 0X81;
        __delay_cycles(4000000);
        P2OUT = 0X18;
        __delay_cycles(4000000);
    }
}

#pragma vector=PORT_VECTOR
__interrupt void Boton(void){
    P2OUT = Datos[i++];
        if(i == 9)
            i=0;

    P1IFG = 0x00;
    __delay_cycles(8000000);
}
