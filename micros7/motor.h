/*
 * motor.h
 *
 *  Created on: 23 abr 2025
 *      Author: Foxi
 */

#ifndef MOTOR_H_
#define MOTOR_H_
void intToStr(unsigned int num, char *str);

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



#endif /* MOTOR_H_ */