#ifndef STREAM_UI_HPP
#define STREAM_UI_HPP

#include "Game.hpp"
#include <iostream>


struct StreamUI {
  Game *game;
};

void StreamUI_init(StreamUI *ui, Game *game);

// void print_wide_digit(int n);

// void print_wide_letter(char c);

// void print_cell_item(const Cell *cell);

// void Cell_print(const Cell *cell);

void StreamUI_print(const StreamUI* ui, bool show_hidden);

void StreamUI_input(StreamUI* ui);

#endif