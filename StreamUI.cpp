#include "StreamUI.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <iomanip>
#include <vector>
#include <limits>

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
    if (cell->num_adjacent_traps > 0) {
      std::cout << colors[cell->num_adjacent_traps];
      print_wide_digit(cell->num_adjacent_traps);
    }
    else {
      std::cout << "  ";
    }
  }
  else if (cell->item == TREASURE) {
    std::cout << "💰";
  }
  else if (cell->item == TRAP) {
    std::cout << "\u001b[48;5;88m"; // red background
    std::cout << "☠️ ";
  }
  else {
    assert(false);
  }
}

void print_cell(const Cell *cell, bool show_hidden) {
  if (show_hidden || cell->state == REVEALED) {
    print_cell_item(cell);
  }
  else if (cell->state == FLAG) {
    std::cout << "\u001b[48;5;7m" << "🚩";
    std::cout << colors[0];
  }
  else if (cell->state == HIDDEN) {
    std::cout << "\u001b[48;5;7m" << "  ";
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

void StreamUI_print_board(const StreamUI* ui, bool show_hidden) {
  Game *game = ui->game;
  for(int r = game->height-1; r >= 0; --r) {
    std::cout << std::setw(2) << r << " ";
    for(int c = 0; c < game->width; c++) {
      const Cell *cell = Game_cell(game, c, r);
      print_cell(cell, show_hidden);
    }
    std::cout << std::endl;
  }
  std::cout << "   ";
  for(int c = 0; c < game->width; c++) {
    print_wide_letter('A' + c);
  }
  std::cout << std::endl;
}

void StreamUI_print_status(const StreamUI* ui) {
  std::cout << Game_num_treasures_found(ui->game)
            << "/" << Game_num_treasures(ui->game)
            << " treasures found." << std::endl;
}

void handle_move_input(StreamUI *ui, std::string move) {
  std::string x_str;
  std::cin >> x_str;
  int x = x_str[0] - 'A';
  int y = -1;
  std::cin >> y;
  if (Game_in_bounds(ui->game, x, y)) {
    if (move == "R") {
      Game_reveal(ui->game, x, y);
    }
    else if (move == "F") {
      Game_toggle_flag(ui->game, x, y);
    }
  }
  else {
    std::cout << "Out of bounds!" << std::endl;
  }
}

void handle_save_input(StreamUI *ui) {
  std::string filename;
  std::cin >> filename;
  std::ofstream out(filename);
  Game_save(ui->game, out);
}

bool StreamUI_input(StreamUI *ui) {
  std::cout << "Enter move: ";
  std::string move;
  std::cin >> move;
  if (move == "q") {
    return false;
  }
  else if (move == "S") {
    handle_save_input(ui);
  }
  else if (move == "R" || move == "F") {
    handle_move_input(ui, move);
  }
  else {
    std::cout << "Invalid move!" << std::endl;
  }

  // If there was an input error, print a message, reset cin errors, and clear buffer.
  if (!std::cin) {
    std::cout << "Invalid input!" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return true;
}

void StreamUI_play(StreamUI *ui) {
  do {
    StreamUI_print_board(ui, false);
    StreamUI_print_status(ui);
  }
  while (!Game_is_over(ui->game) && StreamUI_input(ui));


  StreamUI_print_board(ui, true);
  if (Game_num_treasures_found(ui->game) == Game_num_treasures(ui->game)) {
    std::cout << "Yarrr! Ye found all " << Game_num_treasures(ui->game)
              << " treasures."<< std::endl;
  }
  else {
    std::cout << "Avast! Ye hit a trap!" << std::endl;
    std::cout << "Ye found " << Game_num_treasures_found(ui->game) << "/"
              << Game_num_treasures(ui->game) << " treasures." << std::endl;
  }
}