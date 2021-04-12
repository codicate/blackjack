#include <iostream>
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
  explicit Player(int index) : player_index(index) {};

  int hit()
  {
    while (true)
    {
      int i = rand() % DECK_SIZE;

      if (!deck[i])
      {
        card_index = i;
        deck[i] = true;
        const int rank_index = i % 13 + 1;

        // Use emscripten to execute the append card function, which will add the new card to the respective card list on screen.
        EM_ASM({appendCard($0, $1)}, player_index, card_index);

        // If the new card is a A, save to the A's bank, and the true sum will be calculated later
        if (rank_index == 1)
          ++num_of_As;
        else if (rank_index > 9)
          sum += 10;
        else
          sum += rank_index;

        return check_sum();
      }
    }
  }

  int check_sum()
  {
    const int sum_after_As = convert_As();

    // Exceeding 21, lost
    if (sum_after_As > 21)
      return 1;
      // Equal 21, blackjack
    else if (sum_after_As == 21)
      return 2;
      // Neither, continue the game
    else
      return 0;
  }

  int convert_As()
  {
    int sum_after_As = sum;

    if (num_of_As >= 1)
    {
      // Add 11 or 1 to the sum depending on if the sum will go over 21 or not
      sum_after_As += sum + 11 <= 21 ? 11 : 1;

      // For every other A's, if they are 11, it will go over 21, so they must be 1's
      for (int i = num_of_As - 1; i > 0; i--)
        sum_after_As += 1;
    }

    return sum_after_As;
  }
};

const int LOOP_SPEED = 60;
const bool INF_LOOP = true;

Player computer(1);
Player player(2);

int main()
{
  cout << "Game Started" << endl;
  srand(time(nullptr));

  // First card, both player always hit, and there's no need for win checking
  computer.hit();
  player.hit();

  // Second card, both player always hit
  play_round(1);

  // Start the main loop, which will check for player input every 1/60 of a second
  emscripten_set_main_loop(get_input, LOOP_SPEED, INF_LOOP);
}

void play_round(int player_choice)
{
  const int computer_condition = computer.convert_As() < 21 ? computer.hit() : 0;
  const int player_condition = player_choice == 1 ? player.hit() : 0;
  const bool we_have_a_winner = check_absolute_victor(computer_condition, player_condition);

  if (we_have_a_winner)
  {
    // Stop checking the input to make sure the player cannot interact with the game any further
    emscripten_cancel_main_loop();
    cout << "Game Over" << endl;
  }
}

bool check_absolute_victor(int computer_condition, int player_condition)
{
  // If both player choose to stand, continue the game
  if (computer_condition == 0 && player_condition == 0)
    return false;

    // If both player exceeded 21, declare mutual defeat
  else if (computer_condition == 1 && player_condition == 1)
    EM_ASM({announceVictor(3)});

    // If both player got blackjack, declare mutual victory
  else if (computer_condition == 2 && player_condition == 2)
    EM_ASM({announceVictor(4)});

    // Otherwise, the winner is whoever got a blackjack, or whoever did not exceed 21
  else if (computer_condition == 2 || player_condition == 1)
    EM_ASM({announceVictor(1)});
  else
    EM_ASM({announceVictor(2)});

  // Set a 3 seconds timeout before refreshing the page, thus starting a new game
  // During the 3 seconds, player cannot make any other interaction to the game
  EM_ASM({setTimeout(function(){ location.reload() }, 3000)});
  return true;
}

void get_input()
{
  // Call the JS function to get which button is clicked by the player
  int player_choice = EM_ASM_INT({return getInputFromJS()});

  // If player_choice is not 0, pass that choice to the next round. Otherwise, repeat the check
  if (player_choice != 0) play_round(player_choice);
}