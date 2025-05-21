#ifndef	Motor
#define Motor
void configuracion(){
    WDTCTL = WDTPW | WDTHOLD; 

    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    P1DIR = 0xFF;
    P1SEL = 0x00;
    P1SEL2 = 0x00;    
    return;
}


#endif