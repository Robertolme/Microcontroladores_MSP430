const pickrContainer = document.querySelector('.pickr-container');
const colorPreview = document.getElementById('colorPreview');
const message = document.getElementById('message');

// Crear elemento para montar Pickr
const el = document.createElement('div');
pickrContainer.appendChild(el);

// Inicializar Pickr con el tema 'classic'
const pickr = new Pickr({
  el: el,
  theme: 'classic',
  default: '#3498db',
  swatches: [
    'rgba(244, 67, 54, 1)', 'rgba(233, 30, 99, 0.95)',
    'rgba(156, 39, 176, 0.9)', 'rgba(103, 58, 183, 0.85)',
    'rgba(63, 81, 181, 0.8)', 'rgba(33, 150, 243, 0.75)',
    'rgba(3, 169, 244, 0.7)', 'rgba(0, 188, 212, 0.7)'
  ],
  components: {
    preview: true,
    hue: true,
    interaction: {
        rgba: true,
      input: true,
      clear: true
    }
  }
});

// Función para enviar el color al servidor
function enviarColor(rgb) {
  fetch('/color', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ color: rgb })
  })
  .then(res => res.ok ? "Color enviado correctamente." : "Error al enviar color.")
  .then(msg => {
    message.textContent = msg;
    setTimeout(() => message.textContent = '', 2000);
  });
}

// Evento que se activa en cada cambio de color
pickr.on('change', (color) => {
  const rgb = color.toRGBA().toString(0);
  colorPreview.style.backgroundColor = rgb;
  enviarColor(rgb); // Enviar en cada cambio
});
