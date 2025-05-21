#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Detener el watchdog timer
    P1DIR |= BIT0;             // Configurar P1.0 como salida

    // Configurar Timer_A para 1 kHz
    TA0CCR0 = 500;             // Periodo (ajustar según frecuencia deseada)
    TA0CCTL0 = CCIE;           // Habilitar interrupciones
    TA0CTL = TASSEL_2 | MC_1;  // SMCLK, modo up

    __bis_SR_register(GIE);    // Habilitar interrupciones globales

    while (1);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    P1OUT ^= BIT0;  // Invertir el estado del pin P1.0
}
