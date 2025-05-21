#include <msp430.h>
#include "lcd.h"
#include "motor.h"

unsigned int cuenta, RPS;
char rps[5];

void velocidad(){
    clear();
    gotoxy(1,2);
    intToStr(RPS, rps);
    imprimir(rps);
    imprimir(" RPS");
    __delay_cycles(800000);
}

int main(void){

   WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
   //Configurar la CPU a diferentes velocidades (1, 8, 12 o 16 MHZ)
   BCSCTL1 = CALBC1_16MHZ;
   DCOCTL = CALDCO_16MHZ ;

   TA1CTL =TASSEL_1+MC_1+TAIE;
   TA1CCR0= 32767;

   TA0CTL = TASSEL_1+MC_1;
   TA0CCR0 = 327;
   TA0CCTL1 = OUTMOD_7;
   TA0CCR1 = 164; //incrementar de 1 en 1 hasta 327 TA0CCR1++ poner en la interrupcion if(TA0CCR1 <= 327) otro para decrementar

   P1DIR |= BIT2;
   P1SEL |= BIT2;
   P1SEL2 &= ~BIT2;

   __bis_SR_register(GIE);

   P1DIR |=  BIT0;

   P1DIR &= ~BIT6;
   P1REN |=  BIT6;
   P1OUT &= ~BIT6;
   P1IE  |=  BIT6;
   P1IES &= ~BIT6;
   P1IFG =   0x00;

   P1DIR &= ~(BIT5 + BIT4);
   P1SEL &= ~(BIT5 + BIT4);
   P1SEL2 &= ~(BIT5 + BIT4);
   P1REN |= (BIT5 + BIT4);
   P1OUT &= ~(BIT5 + BIT4);
   P1IE |= (BIT5 + BIT4);
   P1IES &= ~(BIT5 + BIT4);
   P1IFG = 0x00;

   //mostrar datos en LCD

   Ini_Lcd();
  while(1)
  {
    velocidad();
  }

}

//Interrupciones
#pragma vector = TIMER1_A1_VECTOR;  //se le otorga una dirección de memoria a la interrupcion
__interrupt void segundo (void)
{
  switch(TA1IV)
  { case 2: break; //TACCR1
    case 4: break; //TACCR2
    case 10:P1OUT ^= BIT0;
            RPS = cuenta;
            cuenta=0;
            break; //TA1FG
  }
}



#pragma vector = PORT1_VECTOR;  //se le otorga una dirección de memoria a la interrupcion
__interrupt void pulso (void)
{  if((P1IFG & BIT6) == BIT6) cuenta++;

   while((P1IN & BIT4) == BIT4){
        TA0CCR1 += 1;
        if(TA0CCR1 > 325) TA0CCR1 = 325;
        velocidad();
   }
   while((P1IN & BIT5) == BIT5){
        TA0CCR1 -= 1;
        if(TA0CCR1 < 3) TA0CCR1 = 3;
        velocidad();
   }

   P1IFG = 0x00;
}