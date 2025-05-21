#ifndef	MOTO_H
#define MOTO_H

//Puertos
#define Sensor_1 (P1IN & BIT0)
#define Sensor_2 (P1IN & BIT2)
#define Sensor_3 (P1IN & BIT3)

#define Sensor_5 (P2IFG & BIT0)
#define Sensor_4 (P2IFG & BIT5)
#define Boton_1 (P2IN & BIT4)

#endif
