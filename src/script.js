import Module from './main.js';

(async () => {
  const em = await Module();

  const numberSpan = document.querySelector('#number');
  const square = em.cwrap('square', 'number', ['number']);
  numberSpan.textContent = square(5);
})();