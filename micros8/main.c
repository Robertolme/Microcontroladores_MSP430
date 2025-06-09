#include <msp430.h>
#include "uart.h"

unsigned int tiempo=0;
unsigned int distancia=0;
unsigned char modo = 0;
char tiempo_c[10];
char distancia_c[10];
char result[20] = "d:";
char retardo = 0;
int distancia_r; 
char numero = 1;
int referencia = 1;

void puslo(){
    P1OUT |= BIT0;
    __delay_cycles(80);
    TA1R = 0;
    P1OUT &= ~BIT0;
}

void print_uart(){
    if(distancia >= 400){
        print("d:Fuera de rango\n");
        return;
    }
    intToStr(distancia, distancia_c);
    int e;
    for (e = 0; distancia_c[e] != 0 ; e++)
    {
        result[e+2] = distancia_c[e];
    }
    result[e+2] = '\n';
    print(result);
    __delay_cycles(300000);

}

int main(void){

   WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer
   BCSCTL1 = CALBC1_8MHZ;
   DCOCTL = CALDCO_8MHZ ;

   TA1CTL = TASSEL_2 + MC_2 + ID_3 + TAIE;
   TA1CCTL1 = CM_1 + CAP+ CCIE;
   TA1CCTL2 = CM_2+CCIS_1+CAP+CCIE;
   P2DIR &= ~(BIT5 + BIT0);
   P2SEL |= (BIT5 + BIT0);
   P2SEL2 &= ~(BIT5 + BIT0);

   P1SEL |= BIT1 + BIT2;
   P1SEL2 |= BIT1 + BIT2;
   P1DIR &= ~BIT1;
   P1DIR |= BIT2;
   UCA0CTL1 |= UCSSEL_2;

   UCA0BR0 = 0x41;
   UCA0BR1 = 0x03;
   UCA0MCTL = UCBRS1;

   UCA0CTL1 &= ~UCSWRST;
   IE2 |= UCA0RXIE;
 

   P1DIR |= BIT0;
   P1OUT &= ~BIT0;

   __bis_SR_register(GIE);


  while(1){
    
  }

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    retardo++;
    if(retardo != 0)
        return;

    if(UCA0RXBUF == '5' && numero==1){
        print("modo:continuo \n");
        puslo();
        print_uart();
    }


    if(UCA0RXBUF == '4'){
        print("modo:medida\n");
        modo = 1;
    }
    if(UCA0RXBUF == '2' && modo == 1  && numero == 1){ //calibrar
        print("modo:referencia \n");
        puslo();
        print_uart();
        referencia = distancia;
        numero = 0;
    }

    if(UCA0RXBUF == '1'){                                       
        print("modo:altura \n");
        puslo();
        if(distancia >= 400){
            print("d:Fuera de rango\n");
            return;
        }
        intToStr((referencia - distancia), distancia_c);
        int e;
        for (e = 0; distancia_c[e] != 0 ; e++)
        {
            result[e+2] = distancia_c[e];
        }
        result[e+2] = '\n';
        print(result);
        __delay_cycles(300000);
        
        numero = 1;
    }

}


#pragma vector = TIMER1_A1_VECTOR
__interrupt void Eco(void){
    switch(TA1IV){
    case 2:
        TA1R = 0;
        break;
    case 4:
        tiempo = TA1CCR2 - 2174;
        distancia = tiempo/58;
        break;
    case 10:
        break;
    } 
}