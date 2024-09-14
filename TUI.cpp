#include <cassert>
#include <ncurses.h>
#include "Game.hpp"
#include "TUI.hpp"

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

// "Private" function declarations
void TUI_init_curses(TUI *tui);

void TUI_init(TUI *tui, Game *game) {
  tui->game = game;
  tui->cursor_x = Game_width(tui->game) / 2;
  tui->cursor_y = Game_height(tui->game) / 2;
  TUI_init_curses(tui);
}

constexpr int HIDDEN_COLOR = 2 + TRAP;
constexpr int FLAG_COLOR = 3 + TRAP;

void TUI_init_curses(TUI *tui) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  start_color();

  // Initialize colors for each item in Item enum
  init_pair(1+EMPTY, COLOR_WHITE, COLOR_BLACK);
  init_pair(1+ONE, COLOR_BLUE, COLOR_BLACK);
  init_pair(1+TWO, COLOR_CYAN, COLOR_BLACK);
  init_pair(1+THREE, COLOR_GREEN, COLOR_BLACK);
  init_pair(1+FOUR, COLOR_YELLOW, COLOR_BLACK);
  init_pair(1+FIVE, COLOR_RED, COLOR_BLACK);
  init_pair(1+SIX, COLOR_RED, COLOR_BLACK);
  init_pair(1+SEVEN, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(1+EIGHT, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(1+TREASURE, COLOR_WHITE, COLOR_YELLOW);
  init_pair(1+TRAP, COLOR_WHITE, COLOR_RED);
  init_pair(HIDDEN_COLOR, COLOR_BLACK, COLOR_WHITE);
  init_pair(FLAG_COLOR, COLOR_RED, COLOR_WHITE);
  
  // board takes up everything except the last row
  tui->board_window = subwin(stdscr, Game_height(tui->game), Game_width(tui->game), 0, 0);

  // status bar in the last row
  // tui->status_window = subwin(main_window, 1, getmaxx(main_window), getmaxy(main_window) - 1, getbegx(main_window));
}

void render_cell(TUI *tui, const Cell *cell) {
  char c;
  if (cell->state == HIDDEN) {
    wattr_on(tui->board_window, COLOR_PAIR(HIDDEN_COLOR), NULL);
    c = ' ';
  }
  else if (cell->state == FLAG) {
    c = 'F';
    wattr_on(tui->board_window, COLOR_PAIR(FLAG_COLOR), NULL);
  }
  else if (cell->state == REVEALED) {
    wattr_on(tui->board_window, COLOR_PAIR(1+cell->item), NULL);
    if (cell->item == EMPTY) {
      c = ' ';
    }
    else if (Item_is_number(cell->item)) {
      // int x = 1+cell->item;
      // wattr_on(tui->board_window, COLOR_PAIR(x), NULL);
      c = '0' + cell->item;
    }
    else if (cell->item == TREASURE) {
      // wattr_on(tui->board_window, COLOR_PAIR(cell->item), NULL);
      c = '$';
    }
    else if (cell->item == TRAP) {
      // wattr_on(tui->board_window, COLOR_PAIR(cell->item), NULL);
      c = 'X';
    }
    else {
      assert(false);
    }
  }
  else {
    assert(false);
  }
  waddch(tui->board_window, c);
}

void TUI_render(TUI *tui) {
  // Render board
  wrefresh(tui->board_window);
  wclear(tui->board_window);
  for(int y = Game_height(tui->game)-1; y >= 0; --y) {
    for(int x = 0; x < Game_width(tui->game); ++x) {
      const Cell *cell = Game_get(tui->game, x, y);
      render_cell(tui, cell);
    }
    // wmove(tui->board_window, y, 0);
  }

  // for(int r = 0; r < Game_height(tui->game); r++) {
  //   for(int c = 0; c < Game_width(tui->game); c++) {
  //     const Cell *cell = Game_get(tui->game, r, c);
  //     render_cell(tui, cell);
      
  //     // wattr_off(tui->board_window, COLOR_PAIR(cell->item), NULL);
  //   }
  // }
  // wattr_on(tui->board_window, COLOR_PAIR(TRAP), NULL);
  // mvwaddch(tui->board_window, Game_height(tui->game)-1 - tui->cursor_y, tui->cursor_x, '*');
  wmove(tui->board_window, Game_height(tui->game)-1 - tui->cursor_y, tui->cursor_x);
  // wrefresh(tui->status_window);
  wrefresh(tui->board_window);
}

bool TUI_input(TUI *tui) {
  int ch = getch();
  if (ch == 'q') {
    return false;
  }
  else if (ch == KEY_UP) {
    tui->cursor_y = (tui->cursor_y + 1) % Game_height(tui->game);
  }
  else if (ch == KEY_DOWN) {
    tui->cursor_y = (tui->cursor_y - 1 + Game_height(tui->game)) % Game_height(tui->game);
  }
  else if (ch == KEY_LEFT) {
    tui->cursor_x = (tui->cursor_x - 1 + Game_width(tui->game)) % Game_width(tui->game);
  }
  else if (ch == KEY_RIGHT) {
    tui->cursor_x = (tui->cursor_x + 1) % Game_width(tui->game);
  }
  else if (ch == ' ') {
    Game_reveal(tui->game, tui->cursor_x, tui->cursor_y);
  }
  else if (ch == 'f') {
    Game_mark(tui->game, tui->cursor_x, tui->cursor_y);
  }
  return true;
}

void TUI_play(TUI *tui) {
  do {
    TUI_render(tui);
  }
  while (TUI_input(tui));
  endwin();
}