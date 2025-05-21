#include <msp430.h>
#include "main.h"

// Sensor_1 | Sensor_2 | Sensor_3 | Motor2
//	  0           0         0         0
//	  0           0         1         1
//	  1           0         1         1      
//	  1           1         1         0         
//	  1           0         1         0  
//	  0           0         1         1
//	  1           1         0         0           
//    1           0         0         0
//    0           0         0         0 

char  i = 0;

void main(void){	 
	WDTCTL = WDTPW | WDTHOLD; 

    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    //Salidas

    P1DIR |= (BIT1);
    P1SEL &= ~(BIT1);
    P1SEL2 &= ~(BIT1);

    P2DIR |= (BIT1);
    P2SEL &= ~(BIT1);
    P2SEL2 &= ~(BIT1);        

    //Entrada
    P1DIR &= ~(BIT0 | BIT2 | BIT3);
    P1SEL &= ~(BIT0 | BIT2 | BIT3);
    P1SEL2 &= ~(BIT0 | BIT2 | BIT3);

    P1REN |= (BIT0 | BIT2 | BIT3);      
    P1OUT |= (BIT0 | BIT2 | BIT3);

    P2DIR &= ~(BIT0 | BIT4 | BIT5);  
    P2SEL &= ~(BIT0 | BIT4 | BIT5); 
    P2SEL2 &= ~(BIT0 | BIT4 | BIT5); 

    P2REN |= (BIT0 | BIT4 | BIT5);     
    P2OUT |= (BIT0 | BIT4 | BIT5);   

    
    P2IE |= (BIT0 | BIT4 | BIT5); //Permisoa
    P2IES |= (BIT0 | BIT4 | BIT5); //Configurar flanco de subida o bajada. 0 subida , 1 bajada 
    P2IFG = 0x00; //registro

    __bis_SR_register(GIE);

	P1OUT &= ~BIT1;
	P2OUT &= ~BIT1; 
	
	while(1){
        while(Sensor_1 == BIT0 && Sensor_3 == 0){ P2OUT |= BIT1;
        if(Sensor_1 == BIT0) {P1OUT &= ~BIT1;
        i=0;}
        } 
		while(Sensor_2 == 0 || Sensor_3 == BIT3){ P2OUT &= ~BIT1;
        if(Sensor_1 == BIT0) {P1OUT &= ~BIT1;
        i=0;} } 
	}
}

/// Sensor_5  Sensor_4  Motor_1
//      1        0        0
//      0        1        1
//      1        1         1
//      0        1        1

#pragma vector=PORT2_VECTOR
__interrupt void Boton(void){
    if(Sensor_1 == BIT0){P2IFG=0x00; return;}
    if(!(Boton_1)) { i ^= 1;
		P1OUT = (P1OUT & ~BIT1) | (i << 1);
        P2IFG &= ~BIT4;
	}
    if(i == 1){
	    if((Sensor_5  == 0 && Sensor_4 == BIT5)){ P1OUT &= ~BIT1;
        P2IFG &= ~BIT5;} 
		else {P1OUT |= BIT1;
        P2IFG &= ~BIT0;}
	}
    P2IFG=0x00;
}