#include <msp430.h>
#include "lcd.h"

void float_to_str(float num, char *str) {
    int int_part = (int)num;                    
    int parte_decimal = (int)((num - int_part) * 100);  

    if (num < 0) {
        *str++ = '-';
        int_part = -int_part;
        parte_decimal = -parte_decimal;
    }

    int i = 0;
    char temp[10];
    do {
        temp[i++] = (int_part % 10) + '0';
        int_part /= 10;
    } while (int_part > 0);

    while (i > 0) {
        *str++ = temp[--i];
    }

    *str++ = '.';  

    str[0] = (parte_decimal / 10) + '0';
    str[1] = (parte_decimal % 10) + '0';
    str[2] = '\0';
}

int contador = 1;
char disparo[10], disparoi[10] ;
float retardo = 1.0, fase = 1.0;
char enable = 1, apagado = 1;

int main(void){

   WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer
   BCSCTL1 = CALBC1_16MHZ;
   DCOCTL = CALDCO_16MHZ ;

   TA1CTL = TASSEL_2 + MC_0 + ID_3;
   TA1CCR0 = (2000 * retardo) - 1;
   TA1CCTL0 = CCIE;
   fase = (retardo * 0.001) * (180 / 0.0083);

   __bis_SR_register(GIE);

   P1DIR &= ~(BIT5 + BIT4 + BIT3 + BIT6);
   P1SEL &= ~(BIT5 + BIT4 + BIT3 + BIT6);
   P1SEL2 &= ~(BIT5 + BIT4 + BIT3 + BIT6);
   P1REN |= (BIT5 + BIT4 + BIT3 + BIT6);
   P1OUT &= ~(BIT5 + BIT4 + BIT3 + BIT6);
   P1IE |= (BIT5 + BIT4 + BIT3 + BIT6);
   P1IES &= ~(BIT5 + BIT4 + BIT3 + BIT6);
   P1IFG = 0x00;

   P1DIR |= BIT0;
   P1OUT &= ~BIT0;

   Ini_Lcd();
   gotoxy(1,1);
   imprimir("Examen Easy 2");

  while(1)
  {
    if(enable && apagado){
      float_to_str(fase, disparo);
      float_to_str((TA1CCR0 + 1), disparoi);
      gotoxy(1,1);
      imprimir("Disparo: ");
      imprimir(disparoi);
      gotoxy(1,2);
      imprimir("Fase: ");
      imprimir(disparo);
      imprimir("'");
    }
  }

}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void segundo(void)
{
    P1OUT |= BIT0;
    __delay_cycles(320);
    P1OUT &= ~BIT0;
    TA1CTL &= ~MC_1;
}

#pragma vector = PORT1_VECTOR
__interrupt void pulso(void)
{
    if ((P1IFG & BIT3) == BIT3) {
            enable = 1;
            retardo -= (retardo <= 1)? 0 : 0.5;
            TA1CCR0 = (2000 * retardo) - 1;
            fase = (retardo * 0.001) * (180 / 0.0083);
    }

    if ((P1IFG & BIT4) == BIT4) {
            enable = 1;
            retardo += (retardo >= 7)? 0 : 0.5;
            TA1CCR0 = (2000 * retardo) - 1;
            fase = (retardo * 0.001) * (180 / 0.0083);
        }

    if ((P1IFG & BIT6) == BIT6) {
        switch(contador){
                case 1:
                    Ini_Lcd();
                    TA1CTL = 0x00;
                    apagado = 0;
                    gotoxy(1,2);
                    imprimir("----Apagado----");
                    contador = 2;
                    break;
                case 2:
                    Ini_Lcd();
                    gotoxy(1,2);
                    imprimir("Modo Bajo");
                    TA1CTL = TASSEL_2 + MC_0 + ID_3;
                    retardo = 6.0;
                    apagado = 1;
                    enable =0;
                    TA1CCR0 = (2000 * retardo) - 1;
                    fase = (retardo * 0.001) * (180 / 0.0083);
                    contador = 3;
                    break;
                case 3:
                    Ini_Lcd();
                    gotoxy(1,2);
                    imprimir("Modo Medio");
                    TA1CTL = TASSEL_2 + MC_0 + ID_3;
                    retardo = 4.0;
                    apagado = 1;
                    enable = 0;
                    TA1CCR0 = (2000 * retardo) - 1;
                    fase = (retardo * 0.001)* (180 / 0.0083);
                    contador = 4;
                    break;
                case 4:
                    Ini_Lcd();
                    gotoxy(1,2);
                    imprimir("Modo Alto");
                    TA1CTL = TASSEL_2 + MC_0 + ID_3;
                    retardo = 2.0;
                    apagado = 1;
                    enable = 0;
                    TA1CCR0 = (2000 * retardo) - 1;
                    fase = (retardo * 0.001) * (180 / 0.0083);
                    contador = 5;
                    break;
                default:
                    Ini_Lcd();
                    gotoxy(1,1);
                    imprimir("Examen easy 2");
                    enable = 1;
                    apagado = 1;
                    contador = 1;
                    break;
                }
    }

    if ((P1IFG & BIT5) == BIT5) {
        TA1R = 0;
        TA1CTL |= MC_1;
    }

    P1IFG = 0x00;
}