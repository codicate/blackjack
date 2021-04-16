const resultSpan = document.querySelector('#result');
const computerCardDisplay = document.querySelector('#computer');
const playerCardDisplay = document.querySelector('#player');
const hitBtn = document.querySelector('#hit');
const standBtn = document.querySelector('#stand');

const computerHand = [];
const playerHand = [];

const gameOver = false;

const getCorrespondingCardImg = (cardIndex) =>
  `/assets/card_${Math.floor(cardIndex / 13 + 1)}_${cardIndex % 13 + 1}.png`;

const appendCard = (playerIndex, cardIndex) => {
  const cardDisplay = playerIndex == 1 ? computerCardDisplay : playerCardDisplay;
  const hand = playerIndex == 1 ? computerHand : playerHand;
  hand.push(cardIndex);

  cardDisplay.innerHTML = hand.map((cardIndex) =>
    `<img src=${getCorrespondingCardImg(cardIndex)}>`
  ).join(' ');

  computerCardDisplay.children[0].setAttribute('src',
    '/assets/card_back.png'
  );
};

const announceVictor = (condition) => {
  computerCardDisplay.children[0].setAttribute('src',
    getCorrespondingCardImg(computerHand[0])
  );

  if (condition == 3) resultSpan.textContent = 'You both lost';
  else if (condition == 4) resultSpan.textContent = 'It\'s a tie!';
  else resultSpan.textContent = (condition == 1 ? 'computer' : 'player') + ' won!!!!';
};

let playerDecision = 0;

hitBtn.onclick = () => {
  playerDecision = 1;
};

standBtn.onclick = () => {
  playerDecision = 2;
};

const getInputFromJS = () => {
  input = playerDecision;
  playerDecision = 0;
  return input;
};