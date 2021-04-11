#include <iostream>
using namespace std;

#include <emscripten/emscripten.h>

int main()
{
  cout << "bro" << endl;
}

extern "C"
{
  EMSCRIPTEN_KEEPALIVE
  int square(int number)
  {
    return number * number;
  }
}