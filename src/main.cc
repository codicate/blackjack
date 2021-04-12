#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

#include <emscripten.h>

void get_input();
void play_round(int);
bool check_absolute_victor(int, int);

struct Player
{
  static constexpr int DECK_SIZE = 52;
  inline static bool deck[DECK_SIZE] = {false};

  int sum = 0, num_of_As = 0;
  int card_index = 0, rank_value = 0;

  int player_index;
  explicit Player(int index) : player_index(index){};

  // Draw a random card
  int draw()
  {
    while (true)
    {
      int i = rand() % DECK_SIZE;

      if (!deck[i])
      {
        card_index = i;
        deck[i] = true;

        const int rank_index = i % 13 + 1;

        if (rank_index == 1)
          ++num_of_As;
        else if (rank_index > 9)
          rank_value = 10;
        else
          rank_value = rank_index;

        sum += rank_value;
        convert_As();

        EM_ASM({appendCard($0, $1)}, player_index, card_index);

        return check_sum();
      }
    }
  }

  void convert_As()
  {
    if (num_of_As > 1)
    {
      if (sum + 11 <= 21)
        sum += 11;
      else
        sum += 1;

      for (int i = num_of_As - 1; i > 0; i--)
      {
        sum += 1;
      }
    }
  }

  int check_sum()
  {
    const string name = player_index == 1 ? "computer" : "player";

    cout << name << " sum: " << sum << endl;

    if (sum > 21)
      return 1;
    else if (sum == 21)
      return 2;
    else
      return 0;
  }
};

const int LOOP_SPEED = 60;
const bool INF_LOOP = true;

int num;

Player computer(1);
Player player(2);

int main()
{
  cout << "main" << endl;
  srand(time(nullptr));

  computer.draw();
  player.draw();

  play_round(1);
  emscripten_set_main_loop(get_input, LOOP_SPEED, INF_LOOP);
}

void play_round(int num)
{
  const bool we_have_a_winner = check_absolute_victor(computer.draw(), player.draw());

  if (!we_have_a_winner)
  {
    cout << "continue playing" << endl;
  }
  else
  {
    emscripten_cancel_main_loop();
    cout << "game over" << endl;
  }
}

bool check_absolute_victor(int computer_condition, int player_condition)
{
  if (computer_condition == 0 && player_condition == 0)
    return false;
  else if (computer_condition == 1 && player_condition == 1)
    EM_ASM({announceVictor(3)});
  else if (computer_condition == 2 && player_condition == 2)
    EM_ASM({announceVictor(4)});
  else if (computer_condition == 2 || player_condition == 1)
    EM_ASM({announceVictor(1)});
  else
    EM_ASM({announceVictor(2)});

  EM_ASM({setTimeout(function(){location.reload()}, 8000)});
  return true;
}

void get_input()
{
  num = EM_ASM_INT({return getInputFromJS()});

  if (num != 0)
  {
    play_round(num);
    num = 0;
  }
}