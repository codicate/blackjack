#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// #include <emscripten/bind.h>
// using namespace emscripten;

inline static const string RANKS[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
inline static const string SUITS[4] = {"spades", "hearts", "clubs", "diamonds"};

struct Player
{
  static constexpr int DECK_SIZE = 52;
  inline static bool deck[DECK_SIZE] = {false};

  vector<int> hand;

  int player_index, card_index{}, rank_index{}, suit_index{};
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
        rank_index = card_index % 13;
        suit_index = card_index / 13;
        deck[i] = true;
        return i;
      }
    }
  }

  bool deal()
  {
    // Draw a random card
    int drawnCard = draw();
    hand[drawnCard] = false;
    return true;
  }
};

int main()
{
  srand(time(nullptr));
  Player player(1);
  Player computer(2);

  const int card = player.draw();
  cout << card;
}