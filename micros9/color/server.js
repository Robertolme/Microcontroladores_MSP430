const express = require('express');
const { SerialPort } = require('serialport');

const app = express();
const port = 3000;

app.use(express.static('public'));
app.use(express.json());

// Configurar el puerto serie UART
const uart = new SerialPort({
  path: '/dev/ttyACM1',
  baudRate: 9600, // Ajusta si usas otro baudrate
  autoOpen: true
});

uart.on('open', () => console.log('UART abierto en /dev/ttyMA1'));
uart.on('error', err => console.error('Error en UART:', err.message));

app.post('/color', (req, res) => {
  const { color } = req.body;
  console.log('Color recibido:', color);

  const match = color.match(/rgba?\((\d+),\s*(\d+),\s*(\d+)/);
  if (!match) {
    return res.status(400).send('Formato de color inválido');
  }

  const r = match[1].padStart(3, '0');
  const g = match[2].padStart(3, '0');
  const b = match[3].padStart(3, '0');

  const mensaje = `R${r}\nG${g}\nB${b}\n`;
  console.log('Enviando por UART:', mensaje);

  uart.write(mensaje, (err) => {
    if (err) {
      console.error('Error al escribir en UART:', err.message);
      return res.status(500).send('Error al enviar por UART');
    }
    res.sendStatus(200);
  });
});

app.listen(port, () => {
  console.log(`Servidor escuchando en http://localhost:${port}`);
});
