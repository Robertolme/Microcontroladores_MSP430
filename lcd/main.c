#include <msp430.h>
#include "lcd.h"

int main(void)
{
   WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
   //Configurar la CPU a diferentes velocidades (1, 8, 12 o 16 MHZ)
   BCSCTL1 = CALBC1_16MHZ;
   DCOCTL = CALDCO_16MHZ ;

   Ini_Lcd();
   Cmd_Lcd(0x83);
   imprimir("Hola tonotos");

  while(1){
   
   }

}


