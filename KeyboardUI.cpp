#include <cassert>
#include <ncurses.h>
#include "Game.hpp"
#include "KeyboardUI.hpp"

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
void KeyboardUI_init_curses(KeyboardUI *ui);

void KeyboardUI_init(KeyboardUI *ui, Game *game) {
  ui->game = game;
  ui->cursor_x = Game_width(ui->game) / 2;
  ui->cursor_y = Game_height(ui->game) / 2;
  KeyboardUI_init_curses(ui);
}
constexpr int COLOR_TREASURE = 9;
constexpr int COLOR_TRAP = 10;
constexpr int COLOR_HIDDEN = 11;
constexpr int COLOR_FLAG = 12;
constexpr int COLOR_EMPTY = 13;

void KeyboardUI_init_curses(KeyboardUI *ui) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  start_color();

  // Initialize colors for each item in Item enum
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_RED, COLOR_BLACK);
  init_pair(6, COLOR_RED, COLOR_BLACK);
  init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(8, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_TREASURE, COLOR_WHITE, COLOR_YELLOW);
  init_pair(COLOR_TRAP, COLOR_WHITE, COLOR_RED);
  init_pair(COLOR_HIDDEN, COLOR_BLACK, COLOR_WHITE);
  init_pair(COLOR_FLAG, COLOR_RED, COLOR_WHITE);
  init_pair(COLOR_EMPTY, COLOR_WHITE, COLOR_BLACK);
  
  // board takes up everything except the last row
  ui->board_window = subwin(stdscr, Game_height(ui->game), Game_width(ui->game), 0, 0);

  // status bar in the last row
  // ui->status_window = subwin(main_window, 1, getmaxx(main_window), getmaxy(main_window) - 1, getbegx(main_window));
}

void render_cell(KeyboardUI *ui, const Cell *cell) {
  char c;
  if (cell->state == HIDDEN) {
    wattr_on(ui->board_window, COLOR_PAIR(COLOR_HIDDEN), NULL);
    c = ' ';
  }
  else if (cell->state == FLAG) {
    wattr_on(ui->board_window, COLOR_PAIR(COLOR_FLAG), NULL);
    c = 'F';
  }
  else if (cell->state == REVEALED) {
    if (cell->item == EMPTY) {
      if (cell->num_adjacent_traps == 0) {
        wattr_on(ui->board_window, COLOR_PAIR(COLOR_EMPTY), NULL);
        c = ' ';
      }
      else {
        wattr_on(ui->board_window, COLOR_PAIR(cell->num_adjacent_traps), NULL);
        c = '0' + cell->num_adjacent_traps;
      }
    }
    else if (cell->item == TREASURE) {
      wattr_on(ui->board_window, COLOR_PAIR(COLOR_TREASURE), NULL);
      c = '$';
    }
    else if (cell->item == TRAP) {
      wattr_on(ui->board_window, COLOR_PAIR(COLOR_TRAP), NULL);
      c = 'X';
    }
    else {
      assert(false);
    }
  }
  else {
    assert(false);
  }
  waddch(ui->board_window, c);
}

void KeyboardUI_render(KeyboardUI *ui) {
  // Render board
  wrefresh(ui->board_window);
  wclear(ui->board_window);
  for(int y = Game_height(ui->game)-1; y >= 0; --y) {
    for(int x = 0; x < Game_width(ui->game); ++x) {
      const Cell *cell = Game_cell(ui->game, x, y);
      render_cell(ui, cell);
    }
    // wmove(ui->board_window, y, 0);
  }

  // for(int r = 0; r < Game_height(ui->game); r++) {
  //   for(int c = 0; c < Game_width(ui->game); c++) {
  //     const Cell *cell = Game_cell(ui->game, r, c);
  //     render_cell(ui, cell);
      
  //     // wattr_off(ui->board_window, COLOR_PAIR(cell->item), NULL);
  //   }
  // }
  // wattr_on(ui->board_window, COLOR_PAIR(TRAP), NULL);
  // mvwaddch(ui->board_window, Game_height(ui->game)-1 - ui->cursor_y, ui->cursor_x, '*');
  wmove(ui->board_window, Game_height(ui->game)-1 - ui->cursor_y, ui->cursor_x);
  // wrefresh(ui->status_window);
  wrefresh(ui->board_window);
}

bool KeyboardUI_input(KeyboardUI *ui) {
  int ch = getch();
  if (ch == 'q') {
    return false;
  }
  else if (ch == KEY_UP) {
    ui->cursor_y = (ui->cursor_y + 1) % Game_height(ui->game);
  }
  else if (ch == KEY_DOWN) {
    ui->cursor_y = (ui->cursor_y - 1 + Game_height(ui->game)) % Game_height(ui->game);
  }
  else if (ch == KEY_LEFT) {
    ui->cursor_x = (ui->cursor_x - 1 + Game_width(ui->game)) % Game_width(ui->game);
  }
  else if (ch == KEY_RIGHT) {
    ui->cursor_x = (ui->cursor_x + 1) % Game_width(ui->game);
  }
  else if (ch == ' ') {
    Game_reveal(ui->game, ui->cursor_x, ui->cursor_y);
  }
  else if (ch == 'f') {
    Game_toggle_flag(ui->game, ui->cursor_x, ui->cursor_y);
  }
  return true;
}

void KeyboardUI_play(KeyboardUI *ui) {
  do {
    KeyboardUI_render(ui);
  }
  while (KeyboardUI_input(ui));
  endwin();
}