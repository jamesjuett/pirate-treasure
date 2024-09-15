#ifndef TUI_HPP
#define TUI_HPP

#include <ncurses.h>
#include "Game.hpp"

struct TUI {
  Game *game;
  WINDOW *board_window;
  WINDOW *status_window;
  int cursor_x;
  int cursor_y;
};

void TUI_init(TUI *tui, Game *game);
void TUI_play(TUI *tui);

#endif