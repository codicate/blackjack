#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

#include <emscripten.h>

void get_input();
void end();

struct Player
{
  static constexpr int DECK_SIZE = 52;
  inline static bool deck[DECK_SIZE] = {false};

  vector<int> hand;

  int player_index, card_index{};

  // Draw a random card
  void draw()
  {
    while (true)
    {
      int i = rand() % DECK_SIZE;

      if (!deck[i])
      {
        card_index = i;
        deck[i] = true;
        hand.push_back(card_index);
        EM_ASM({appendCard($0)}, card_index);
        return;
      }
    }
  }

  void get_starting_cards()
  {
    for (int i = 0; i < 2; i++)
      draw();
  }

  explicit Player(int index) : player_index(index), hand()
  {
    get_starting_cards();
  };
};

const int LOOP_SPEED = 60;
const bool INF_LOOP = true;

int num;

int main()
{
  cout << "main" << endl;
  srand(time(nullptr));
  emscripten_set_main_loop(get_input, LOOP_SPEED, INF_LOOP);
}

void get_input()
{
  num = EM_ASM_INT({ return input; });

  if (num != 0)
  {
    emscripten_cancel_main_loop();
    end();
  }
}

void end()
{
  cout << "num: " << num << endl;

  Player player(1);
  Player computer(2);

  for (int i : player.hand)
  {
    cout << "i: " << i << endl;
  }

  for (int i : computer.hand)
  {
    cout << "i: " << i << endl;
  }

  EM_ASM({
    setTimeout(() => location.reload(), 3000);
  });
}