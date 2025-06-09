#ifndef UART_H
#define UART_H

void intToStr(unsigned int num, char *str) {
    char *ptr = str;
    char *start = str;

    // Caso especial para 0
    if (num == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    // Construir el número al revés
    while (num > 0) {
        *ptr++ = (num % 10) + '0';
        num /= 10;
    }
    *ptr = '\0';

    // Invertir la cadena en su lugar
    char *end = ptr - 1;
    while (start < end) {
        char temp = *start;
        *start++ = *end;
        *end-- = temp;
    }
}

void print(char *argv){
  volatile int i = 0;
  for (i = 0; argv[i] != 0; i++)
  {
    UCA0TXBUF = argv[i];
    while (!(IFG2&UCA0TXIFG)){}
  }
}



#endif