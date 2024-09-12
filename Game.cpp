#include "Game.hpp"
#include <cassert>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>

void Game_place_items(Game *game, int n, Item item);
int count_items(Game *game, Item item);
void check_invariants(Game *game);
void Game_number_cells(Game *game);
std::vector<std::pair<int,int>> Game_neighbors(const Game* game, int r, int c, bool include_diagonals);
int Game_count_adjacent_items(const Game *game, int r, int c, Item item);


bool Item_is_number(Item item) {
  return EMPTY <= item && item <= EIGHT;
}

void Game_init(Game* game, int height, int width, int num_treasures, int num_traps) {
  game->width = width;
  game->height = height;
  game->cells = std::vector<std::vector<Cell>>(
    height, std::vector<Cell>(width, {EMPTY, HIDDEN})
  );

  game->num_treasures = num_treasures;
  game->num_traps = num_traps;
  game->game_over = false;

  Game_place_items(game, num_treasures, TREASURE);
  Game_place_items(game, num_traps, TRAP);
  Game_number_cells(game);

  check_invariants(game);
}

void Game_place_items(Game *game, int n, Item item) {
  int num_placed = 0;
  while(num_placed < n) {
    int r = rand() % game->height;
    int c = rand() % game->width;
    Cell *cell = Game_get(game, r, c);
    if(cell->item == EMPTY) {
      cell->item = item;
      ++num_placed;
    }
  }
}

void Game_number_cells(Game *game) {
  
  for(int r = 0; r < game->height; r++) {
    for(int c = 0; c < game->width; c++) {
      Cell *cell = Game_get(game, r, c);
      if(cell->item == EMPTY) {
        int n_traps = Game_count_adjacent_items(game, r, c, TRAP);
        assert(0 <= n_traps && n_traps <= 8);
        if (n_traps != 0) {
          cell->item =  static_cast<Item>(n_traps);
        }
        // else leave EMPTY
      }
    }
  }
}

int count_items(Game *game, Item item) {
  int count = 0;
  for(int r = 0; r < game->height; r++) {
    for(int c = 0; c < game->width; c++) {
      if(Game_get(game, r, c)->item == item) {
        ++count;
      }
    }
  }
  return count;
}

void check_invariants(Game *game) {
  assert(game->cells.size() == game->height);
  for(int r = 0; r < game->height; r++) {
    assert(game->cells[r].size() == game->width);
  }
  
  // Count number of treasures
  assert(count_items(game, TREASURE) == game->num_treasures);
  // Count number of traps
  assert(count_items(game, TRAP) == game->num_traps);
}

int Game_width(const Game *game) {
  return game->width;
}

int Game_height(const Game *game) {
  return game->height;
}

bool Game_in_bounds(const Game* game, int r, int c) {
  return 0 <= r && r < game->height && 0 <= c && c < game->width;
}

int Game_num_treasures(const Game *game) {
  return game->num_treasures;
}

int Game_num_traps(const Game *game) {
  return game->num_traps;
}

Cell * Game_get(Game* game, int r, int c) {
  assert(0 <= r && r < game->height);
  assert(0 <= c && c < game->width);
  return &game->cells[r][c];
}

const Cell * Game_get(const Game* game, int r, int c) {
  assert(0 <= r && r < game->height);
  assert(0 <= c && c < game->width);
  return &game->cells[r][c];
}

const std::vector<std::pair<int,int>> NSEW_OFFSETS = {
  {-1, 0},
  {1, 0},
  {0, -1},
  {0, 1},
};

const std::vector<std::pair<int,int>> DIAGONAL_OFFSETS = {
  {-1, -1},
  {-1, 1},
  {1, -1},
  {1, 1},
};

std::vector<std::pair<int,int>> Game_neighbors(const Game* game, int r, int c, bool include_diagonals) {
  std::vector<std::pair<int,int>> neighbors;
  for(const std::pair<int,int> &offset : NSEW_OFFSETS) {
    int dr = offset.first;
    int dc = offset.second;
    if(Game_in_bounds(game, r + dr, c + dc)) {
      neighbors.push_back({r + dr, c + dc});
    }
  }
  if(include_diagonals) {
    for(const std::pair<int,int> &offset : DIAGONAL_OFFSETS) {
      int dr = offset.first;
      int dc = offset.second;
      if(Game_in_bounds(game, r + dr, c + dc)) {
        neighbors.push_back({r + dr, c + dc});
      }
    }
  }
  return neighbors;
}

void Game_reveal(Game* game, int r, int c) {

  Cell *cell = Game_get(game, r, c);

  // do nothing if already revealed
  if (cell->state == REVEALED) {
    return;
  }

  cell->state = REVEALED;
  Game_print(game, false);
  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  if (cell->item == TRAP) {
    game->game_over = true;
  }
  else if (cell->item == TREASURE) {
    ++game->treasures_found;
  }
  else if (cell->item == EMPTY) {
    for(const std::pair<int,int> &neighbor : Game_neighbors(game, r, c, true)) {
      Cell *neighbor_cell = Game_get(game, neighbor.first, neighbor.second);
      if (neighbor_cell->state != REVEALED && Item_is_number(neighbor_cell->item)) {
        Game_reveal(game, neighbor.first, neighbor.second);
      }
    }
  }
}

void Game_mark(Game* game, int r, int c) {
  if (game->cells[r][c].state == HIDDEN) {
    game->cells[r][c].state = FLAG;
  }
  else if (game->cells[r][c].state == FLAG) {
    game->cells[r][c].state = HIDDEN;
  }

  // do nothing if it's REVEALED or GOLD
}

int Game_count_adjacent_items(const Game *game, int r, int c, Item item) {
  int count = 0;
  for(const std::pair<int,int> &neighbor : Game_neighbors(game, r, c, true)) {
    if(Game_get(game, neighbor.first, neighbor.second)->item == item) {
      ++count;
    }
  }
  return count;
}

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

void Game_print(const Game* game, bool show_hidden) {
  for(int r = game->height-1; r >= 0; --r) {
    std::cout << std::setw(2) << r << " ";
    for(int c = 0; c < game->width; c++) {
      const Cell *cell = Game_get(game, r, c);
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