#ifndef KeyboardUI_HPP
#define KeyboardUI_HPP

#include <ncurses.h>
#include "Game.hpp"

struct KeyboardUI {
  Game *game;
  WINDOW *board_window;
  WINDOW *status_window;
  int cursor_x;
  int cursor_y;
};

void KeyboardUI_init(KeyboardUI *ui, Game *game);
void KeyboardUI_play(KeyboardUI *ui);

#endif