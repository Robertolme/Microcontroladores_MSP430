#include <msp430.h> 

const unsigned char Datos[49] = {0xFF, 0x09, 0x09, 0x09, 0x09, 0xF6, 0x00, 0x00, 0xFF, 0x81, 0x81, 0x81, 0xFF, 0x00, 0x00 ,0xFF, 0x61, 0x61, 0x61, 0x6E, 0x00, 0x00 ,0xFF, 0x91, 0x91, 0x81, 0x81, 0x00, 0x00 ,0xFF, 0x09, 0x09, 0x09, 0x09, 0xF6, 0x00, 0x00, 0x01, 0x01, 0x01, 0xFF, 0x01, 0x01, 0x01 ,0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char Datos2[9] = {0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00}; // 1111 1111

char i = 0, Arreglo=0; 

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
    P1IES &= BIT3; //Configurar flanco de subida o bajada. 0 subida , 1 bajada 
    P1IFG = 0x00; //registro

    __bis_SR_register(GIE);

    while (1){
        if(Arreglo == 0){
            P2OUT = Datos[i++];
            if(i == 50) 
                i=0;
        }else{
            P2OUT = Datos2[i++];
            if(i == 9)
                i=0;
        }
        
        __delay_cycles(16000);
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Boton(void){
    Arreglo ^= 1;
    i=0;
    P1IFG &= ~BIT3;
}
