import Module from './test.js';

Module().then((em) => {
  const square = em.cwrap('square', 'number', ['number']);

  const res = square(5);
  console.log(res);
});