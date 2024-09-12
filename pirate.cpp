#include "Game.hpp"

int main() {
  Game game;
  Game_init(&game, 15, 22, 30, 50);

  while(true) {
    Game_print(&game, false);
    std::cout << "Enter row and column: ";
    char first_char;
    char col_char;
    int row;
    bool flag = false;
    std::cin >> first_char;
    if (first_char == '!') {
      std::cin >> col_char >> row;
      flag = true;
    }
    else {
      col_char = first_char;
      std::cin >> row;
    }
    
    int col = col_char - 'A';
    
    if(Game_in_bounds(&game, row, col)) {
      if(flag) {
        Game_mark(&game, row, col);
      }
      else {
        Game_reveal(&game, row, col);
      }
    }
    else {
      std::cout << "Invalid input." << std::endl;
    }
  }

}