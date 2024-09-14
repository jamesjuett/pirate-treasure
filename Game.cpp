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
int Game_count_adjacent_items(const Game *game, const Cell *cell, Item item);


bool Item_is_number(Item item) {
  return EMPTY <= item && item <= EIGHT;
}

void Game_init(Game* game, int width, int height, int num_treasures, int num_traps) {
  game->width = width;
  game->height = height;
  game->cells = std::vector<std::vector<Cell>>(
    width, std::vector<Cell>(height, {EMPTY, HIDDEN})
  );
  for(int x = 0; x < width; x++) {
    for(int y = 0; y < height; y++) {
      Cell *cell = &game->cells[x][y];
      cell->x = x;
      cell->y = y;
    }
  }

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
    int x = rand() % game->width;
    int y = rand() % game->height;
    Cell *cell = Game_get(game, x, y);
    if(cell->item == EMPTY) {
      cell->item = item;
      ++num_placed;
    }
  }
}

void Game_number_cells(Game *game) {
  
  for(int x = 0; x < game->width; x++) {
    for(int y = 0; y < game->height; y++) {
      Cell *cell = Game_get(game, x, y);
      if(cell->item == EMPTY) {
        int n_traps = Game_count_adjacent_items(game, cell, TRAP);
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
  for(int x = 0; x < game->width; x++) {
    for(int y = 0; y < game->height; y++) {
      if(Game_get(game, x, y)->item == item) {
        ++count;
      }
    }
  }
  return count;
}

void check_invariants(Game *game) {
  assert(game->cells.size() == game->width);
  for(int x = 0; x < game->height; x++) {
    assert(game->cells[x].size() == game->height);
  }
  
  assert(count_items(game, TREASURE) == game->num_treasures);
  assert(count_items(game, TRAP) == game->num_traps);
}

int Game_width(const Game *game) {
  return game->width;
}

int Game_height(const Game *game) {
  return game->height;
}

bool Game_in_bounds(const Game* game, int x, int y) {
  return 0 <= x && x < game->width && 0 <= y && y < game->height;
}

int Game_num_treasures(const Game *game) {
  return game->num_treasures;
}

int Game_num_traps(const Game *game) {
  return game->num_traps;
}

Cell * Game_get(Game* game, int x, int y) {
  assert(Game_in_bounds(game, x, y));
  return &game->cells[x][y];
}

const Cell * Game_get(const Game* game, int x, int y) {
  assert(Game_in_bounds(game, x, y));
  return &game->cells[x][y];
}

bool Game_is_game_over(const Game *game) {
  return game->game_over;
}

const std::vector<std::pair<int,int>> NEIGHBOR_OFFSETS = {
  {-1, 0}, // NSEW
  {1, 0},
  {0, -1},
  {0, 1},
  {-1, -1}, // diagonals
  {-1, 1},
  {1, -1},
  {1, 1},
};

std::vector<Cell *> Game_neighbors(Game* game, Cell *cell, bool include_diagonals) {
  std::vector<Cell *> neighbors;
  for(int i = 0; i < (include_diagonals ? 8 : 4); ++i) {
    int dx = NEIGHBOR_OFFSETS[i].first;
    int dy = NEIGHBOR_OFFSETS[i].second;
    if(Game_in_bounds(game, cell->x + dx, cell->y + dy)) {
      neighbors.push_back(Game_get(game, cell->x + dx, cell->y + dy));
    }
  }
  return neighbors;
}

std::vector<const Cell *> Game_neighbors(const Game* game, const Cell *cell, bool include_diagonals) {
  std::vector<const Cell *> neighbors;
  for(int i = 0; i < (include_diagonals ? 8 : 4); ++i) {
    int dx = NEIGHBOR_OFFSETS[i].first;
    int dy = NEIGHBOR_OFFSETS[i].second;
    if(Game_in_bounds(game, cell->x + dx, cell->y + dy)) {
      neighbors.push_back(Game_get(game, cell->x + dx, cell->y + dy));
    }
  }
  return neighbors;
}

void Game_reveal(Game* game, int x, int y) {

  Cell *cell = Game_get(game, x, y);

  // do nothing if already revealed
  if (cell->state == REVEALED) {
    return;
  }

  cell->state = REVEALED;
  // Game_print(game, false);
  // std::this_thread::sleep_for(std::chrono::milliseconds(20));

  if (cell->item == TRAP) {
    game->game_over = true;
  }
  else if (cell->item == TREASURE) {
    ++game->treasures_found;
  }
  else if (cell->item == EMPTY) {
    for(Cell *neighbor : Game_neighbors(game, cell, true)) {
      if (neighbor->state != REVEALED && Item_is_number(neighbor->item)) {
        Game_reveal(game, neighbor->x, neighbor->y);
      }
    }
  }
}

void Game_mark(Game* game, int x, int y) {
  Cell *cell = Game_get(game, x, y);
  if (cell->state == HIDDEN) {
    cell->state = FLAG;
  }
  else if (cell->state == FLAG) {
    cell->state = HIDDEN;
  }

  // do nothing if it's REVEALED
}

int Game_count_adjacent_items(const Game *game, const Cell *cell, Item item) {
  int count = 0;
  for(const Cell *neighbor : Game_neighbors(game, cell, true)) {
    if(neighbor->item == item) {
      ++count;
    }
  }
  return count;
}