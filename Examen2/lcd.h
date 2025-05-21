//**************************
// Programa para controlar un LCD de 16 caracteres por 2 líneas, utilizando tres
// terminales del microcontrolador
//
//                MSP430G2553
//        3.3V -----------------       SN74HC164            LCD 16 x 2
//        /|\ |                 |     ____            ____
//         |--| Vcc         P2.4|--->|CLOCK    Qa|---DB4--> |  _  |-->5V
//            |             P2.3|-+->|A        Qb|---DB5--> | |   | |
//            |                 | |->|B        Qc|---DB6--> | |   | |
//            |                 |    |         Qd|---DB7--> | |   | |
//            |                 |    |         Qe|---RS --> | | L | |
//            |                 |    |           |          | | C | |
//            |                 |    |        CLR|-->5Vcc   | | D | |
//            |                 |    |_____|          | |   | |
//            |                 |                           | |   | |
//            |             P2.2|-------------------- E --> | |_| |RW -->GND
//            |                 |                           |___|----->GND
//            |             Vss |-->GND
//            |                 |
// JULIO 2019  IPN
// EDGAR R CALDERÓN DÍAZ
//**************************
#ifndef LCD16x2c_H_
#define LCD16x2c_H_

char a=0;

void Ini_Lcd(void);
void Cmd_Lcd(unsigned char CMD);
void Dato_Lcd(unsigned char DATO);
void Send_10b(unsigned int Dat_Ser);
void imprimir(char *argv);

#define CLK  BIT4
#define AB   BIT3
#define E    BIT2

//Función de inicialización LCD para un Bus de datos de 4Bits
void Ini_Lcd()
{
  P2SEL &= ~(E+AB+CLK);           //Configuración de las terminales P2.2, P2.3 y P2.4 para controlar el
  P2SEL2 &= ~(E+AB+CLK);          //74HC164 y el LCD
  P2DIR |= E+AB+CLK;
  P2OUT &= ~(E+AB+CLK);

  __delay_cycles(240000);        //Since VDD reaches more than 4.5V wait more than 15ms

                                //Function Set(Interface data length is 8-bit)
  Send_10b(0x063);              //|RS|R/W|DB7|DB6|DB5|DB4|
  __delay_cycles(640);           //| 0| 0 | 0 | 1 | 1 | 0 |
                                //|RS|R/W|DB7|DB6|DB5|DB4|
                                //| 0| 0 | 0 | 0 | 1 | 1 |
                                //Wait more than 40 us

  Send_10b(0x062);              //|RS|R/W|DB7|DB6|DB5|DB4|
  __delay_cycles(640);           //| 0| 0 | 0 | 1 | 1 | 0 |
                                //|RS|R/W|DB7|DB6|DB5|DB4|
                                //| 0| 0 | 0 | 0 | 1 | 0 |
                                //Wait more than 40 us

  Send_10b(0x048);             //|RS|R/W|DB7|DB6|DB5|DB4|
  __delay_cycles(640);      

  Send_10b(0x00C);  //0x00C           //|RS|R/W|DB7|DB6|DB5|DB4|
  __delay_cycles(640);           //| 0| 0 | 0 | 0 | 0 | 0 |
                                //|RS|R/W|DB7|DB6|DB5|DB4|
                                //| 0| 0 | 1 | 1 | 1 | 1 |
                                //Wait more than 40 us
  Cmd_Lcd(0x01);
}


void Cmd_Lcd(unsigned char CMD)
{
  unsigned int Dat10b;
  if ((CMD & 0xFC) <= 0x02)
  {
    Send_10b(CMD);
    __delay_cycles(26240);
  }
  else
  {
    Dat10b = CMD;
    Dat10b = (Dat10b & 0x00F0) << 1;
    CMD = (CMD & 0x0F);
    Dat10b |= CMD;
    Send_10b(Dat10b);
    __delay_cycles(640);
  }
}

void Dato_Lcd(unsigned char DATO)
{
  unsigned int Dat10b;
  Dat10b = DATO;
  Dat10b = (Dat10b & 0x00F0) << 1;
  DATO = (DATO & 0x0F);
  Dat10b |= DATO+BIT9+BIT4;
  Send_10b(Dat10b);
  __delay_cycles(640);
}

void Send_10b(unsigned int Dat_Ser)
{
  for(a=0; a<=9; a++)
  {
    if((Dat_Ser & BIT9) != 0)
    {
      P2OUT |= AB;
      P2OUT |= CLK;
      P2OUT &= ~CLK;
      Dat_Ser <<= 1;
      if(a == 4)
      {
        P2OUT |= E;
        __delay_cycles(640);
        P2OUT &= ~E;
        __delay_cycles(640);
      }
    }
    else
    {
      P2OUT &= ~AB;
      P2OUT |= CLK;
      P2OUT &= ~CLK;
      Dat_Ser <<= 1;
      if(a == 4)
      {
        P2OUT |= E;
        __delay_cycles(640);
        P2OUT &= ~E;
        __delay_cycles(640);
      }
    }
  }
  P2OUT |= E;
  __delay_cycles(640);
  P2OUT &= ~E;
  __delay_cycles(640);
}

void imprimir(char *argv){
  int i = 0;
  for (i = 0; argv[i] != 0; i++)
  {
    Dato_Lcd(argv[i]);
    __delay_cycles(640);
  }
}

void gotoxy(char x, char y){
    unsigned char pos = x;
    pos |= (y == 1)? 0x80 : 0xC0;
    Cmd_Lcd(pos);
}

void clear(){
  Cmd_Lcd(0x01);
}

#endif
