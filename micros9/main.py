import serial
import time

# Abrir el puerto serial (ajusta el nombre del puerto)
ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)  # Cambia 'COM3' por el que aparezca en tu PC

time.sleep(2)  # Esperar a que el MSP430 reinicie (muy común en LaunchPad)

# Enviar datos al MSP430
ser.write(b'R210\n')
ser.write(b'G221\n')
ser.write(b'B023\n')


ser.close()