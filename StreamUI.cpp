#include "StreamUI.hpp"
#include <iostream>
#include <string>
#include <cassert>
#include <iomanip>
#include <vector>

const std::string RESET_COLOR = "\033[0m";

const std::vector<std::string> colors = {
  "\033[0m",  // reset
  "\033[34m", // blue
  "\033[36m", // cyan
  "\033[32m", // green
  "\033[33m", // yellow
  "\033[31m", // red
  "\033[31m", // red
  "\033[35m", // magenta
  "\033[35m", // magenta
};

void print_wide_digit(int n) {
  assert(0 <= n && n <= 9);
  char str[4] = "\uFF10";
  str[2] += n;
  std::cout << str;
}

void print_wide_letter(char c) {
  assert('A' <= c && c <= 'Z');
  char str[4] = "\uFF21";
  str[2] += (c - 'A');
  std::cout << str;
}

void print_cell_item(const Cell *cell) {
  if (cell->item == EMPTY) {
    std::cout << "  ";
  }
  else if (cell->item == TREASURE) {
    std::cout << "💰";
  }
  else if (cell->item == TRAP) {
    std::cout << "\u001b[48;5;88m";
    std::cout << "☠️ ";
  }
  else {
    assert(ONE <= cell->item && cell->item <= EIGHT);
    std::cout << colors[cell->item];
    print_wide_digit(cell->item);
  }
}

void Cell_print(const Cell *cell) {
  if (cell->state == HIDDEN) {
    std::cout << "\u001b[48;5;7m" << "  ";
  }
  else if (cell->state == FLAG) {
    std::cout << "\u001b[48;5;7m" << "🚩";
    std::cout << colors[0];
  }
  else if (cell->state == REVEALED) {
    print_cell_item(cell);
  }
  else {
    assert(false);
  }

  // reset output color
  std::cout << RESET_COLOR;
}

void StreamUI_init(StreamUI *ui, Game *game) {
  ui->game = game;
}

void StreamUI_print(const StreamUI* ui, bool show_hidden) {
  Game *game = ui->game;
  for(int r = game->height-1; r >= 0; --r) {
    std::cout << std::setw(2) << r << " ";
    for(int c = 0; c < game->width; c++) {
      const Cell *cell = Game_get(game, c, r);
      Cell_print(cell);
    }
    std::cout << std::endl;
  }
  std::cout << "   ";
  for(int c = 0; c < game->width; c++) {
    print_wide_letter('A' + c);
  }
  std::cout << std::endl;
}

void StreamUI_input(StreamUI *ui) {
  Game *game = ui->game;
  std::cout << "Enter move: ";
  char first_char;
  char x_char;
  int y;
  bool flag = false;
  std::cin >> first_char;
  if (first_char == '!') {
    std::cin >> x_char >> y;
    flag = true;
  }
  else {
    x_char = first_char;
    std::cin >> y;
  }
  
  int x = x_char - 'A';
  
  if(Game_in_bounds(game, x, y)) {
    if(flag) {
      Game_mark(game, x, y);
    }
    else {
      Game_reveal(game, x, y);
    }
  }
  else {
    std::cout << "Invalid input." << std::endl;
  }
}