# blackjack

Blackjack made with WASM, emscripten, and C++


### Features:

- 100% of the game logic is coded in C++, including all game state

- JS is used for user interactivity and updating the DOM

- Communication between JS and C++ is done through EM_ASM calling JS functions

- C++ wait for user input using emscripten_set_main_loop in an infinite loop

- Bonus: include a node file for compiling that save me a few key stroke


[Got the pixel art from here](https://drawsgood.itch.io/8bit-deck-card-assets)
