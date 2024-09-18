#include "Game.hpp"
#include <cassert>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>


//////////////////////////////////////////////////////////////////////////
// Declarations for "private" Game ADT functions. Only used as helpers  //
// internally and not available as part of the "public" Game interface, //
// because they are not declared in the .hpp header file.               //
//////////////////////////////////////////////////////////////////////////
void place_items(Game *game, int n, Item item);
int count_items(Game *game, Item item);
void check_invariants(Game *game);
void number_cells(Game *game);
int count_adjacent_items(Game *game, Cell *cell, Item item);
std::vector<Cell *> Game_neighbors(Game* game, Cell *cell);

// A private overload of the Game_cell() function that may be used when the
// Game is not const-qualified and allows modification of cells via the returned
// (non-const-qualified) pointer.
Cell * Game_cell(Game *game, int x, int y);


/////////////////////////////////////////////////////////
// Definitions (implementations) of Game ADT Functions //
/////////////////////////////////////////////////////////


void Game_init(Game* game, int width, int height, int num_treasures, int num_traps) {
  game->width = width;
  game->height = height;
  game->cells = std::vector<std::vector<Cell>>(
    width, std::vector<Cell>(height, Cell{})
  );
  for(int x = 0; x < width; x++) {
    for(int y = 0; y < height; y++) {
      *Game_cell(game, x, y) = {x, y, EMPTY, HIDDEN, false, 0};
    }
  }

  game->num_treasures = num_treasures;
  game->num_treasures_found = 0;
  game->num_traps = num_traps;
  game->num_traps_found = 0;

  place_items(game, num_treasures, TREASURE);
  place_items(game, num_traps, TRAP);
  number_cells(game);

  check_invariants(game);
}

void Game_init(Game *game, std::istream &is) {
  is >> game->width;
  is >> game->height;
  game->cells = std::vector<std::vector<Cell>>(
    game->width, std::vector<Cell>(game->height, Cell{})
  );
  for(int x = 0; x < game->width; ++x) {
    for(int y = 0; y < game->height; ++y) {
      is >> *Game_cell(game, x, y);
    }
  }

  game->num_treasures = count_items(game, TREASURE);
  game->num_treasures_found = 0;
  game->num_traps = count_items(game, TRAP);
  game->num_traps_found = 0;

  check_invariants(game);
}

void Game_save(const Game *game, std::ostream &out) {
  out << game->width << " " << game->height << std::endl;
  for(int x = 0; x < game->width; ++x) {
    for(int y = 0; y < game->height; ++y) {
      out << game->cells[x][y] << " ";
    }
    out << std::endl;
  }
}

int Game_width(const Game *game) {
  return game->width;
}

int Game_height(const Game *game) {
  return game->height;
}

int Game_num_treasures(const Game *game) {
  return game->num_treasures;
}

int Game_num_treasures_found(const Game *game) {
  return game->num_treasures_found;
}

int Game_num_traps(const Game *game) {
  return game->num_traps;
}

int Game_num_traps_found(const Game *game) {
  return game->num_traps_found;
}

bool Game_in_bounds(const Game* game, int x, int y) {
  return 0 <= x && x < game->width && 0 <= y && y < game->height;
}

bool Game_is_over(const Game *game) {
  return game->num_traps_found > 0 || game->num_treasures_found == game->num_treasures;
}

Cell * Game_cell(Game* game, int x, int y) {
  assert(Game_in_bounds(game, x, y));
  return &game->cells[x][y];
}

const Cell * Game_cell(const Game* game, int x, int y) {
  assert(Game_in_bounds(game, x, y));
  return &game->cells[x][y];
}

void Game_reveal(Game* game, int x, int y) {
  check_invariants(game);

  Cell *cell = Game_cell(game, x, y);

  // do nothing if already revealed
  if (cell->state == REVEALED) {
    return;
  }

  cell->state = REVEALED;

  if (cell->item == TRAP) {
    ++game->num_traps_found;
    return;
  }
  
  if (cell->item == TREASURE) {
    ++game->num_treasures_found;
    if (Game_is_over(game)) {
      return; // return early if we got all the treasures
    }
  }

  // If an empty or treasure cell is revealed and has no adjacent traps,
  // reveal all adjacent empty or treasure cells as well.
  if (cell->num_adjacent_traps == 0) {
    for(Cell *neighbor : Game_neighbors(game, cell)) {
      if (neighbor->state != REVEALED && (neighbor->item == EMPTY || neighbor->item == TREASURE)) {
        Game_reveal(game, neighbor->x, neighbor->y);
      }
    }
  }
  
  check_invariants(game);
}

void Game_toggle_flag(Game* game, int x, int y) {
  Cell *cell = Game_cell(game, x, y);
  if (cell->state == HIDDEN) {
    cell->state = FLAG;
  }
  else if (cell->state == FLAG) {
    cell->state = HIDDEN;
  }
  // else do nothing if it's REVEALED
}

std::vector<Cell *> Game_neighbors(Game* game, Cell *cell) {
  std::vector<Cell *> neighbors;
  for(int dx = -1; dx <= 1; ++dx) {
    for(int dy = -1; dy <= 1; ++dy) {
      int nx = cell->x + dx;
      int ny = cell->y + dy;
      if (Game_in_bounds(game, nx, ny)) {
        Cell *neighbor = Game_cell(game, nx, ny);
        if (neighbor != cell) { // don't include self
          neighbors.push_back(neighbor);
        }
      }
    } 
  }
  return neighbors;
}

void place_items(Game *game, int n, Item item) {
  int num_placed = 0;
  while(num_placed < n) {
    int x = rand() % game->width;
    int y = rand() % game->height;
    Cell *cell = Game_cell(game, x, y);
    if(cell->item == EMPTY) {
      cell->item = item;
      ++num_placed;
    }
  }
}

void number_cells(Game *game) {
  for(int x = 0; x < game->width; x++) {
    for(int y = 0; y < game->height; y++) {
      Cell *cell = Game_cell(game, x, y);
      int n_traps = count_adjacent_items(game, cell, TRAP);
      assert(0 <= n_traps && n_traps <= 8);
      cell->num_adjacent_traps = n_traps;
    }
  }
}

int count_adjacent_items(Game *game, Cell *cell, Item item) {
  int count = 0;
  for(const Cell *neighbor : Game_neighbors(game, cell)) {
    if(neighbor->item == item) {
      ++count;
    }
  }
  return count;
}

void check_invariants(Game *game) {
  assert(game->cells.size() == game->width);
  for(int x = 0; x < game->width; x++) {
    assert(game->cells[x].size() == game->height);
  }
  assert(count_items(game, EMPTY) + count_items(game, TREASURE) + count_items(game, TRAP) == game->width * game->height);
  
  assert(0 < game->num_treasures);
  assert(0 <= game->num_traps);
  assert(game->num_treasures + game->num_traps < game->width * game->height / 2);
  assert(count_items(game, TREASURE) == game->num_treasures);
  assert(count_items(game, TRAP) == game->num_traps);
  assert(count_items(game, EMPTY) == game->width * game->height - game->num_treasures - game->num_traps);

  assert(0 <= game->num_treasures_found && game->num_treasures_found <= game->num_treasures);
  assert(0 <= game->num_traps_found && game->num_traps_found <= game->num_traps);
}

int count_items(Game *game, Item item) {
  int count = 0;
  for(int x = 0; x < game->width; x++) {
    for(int y = 0; y < game->height; y++) {
      if(Game_cell(game, x, y)->item == item) {
        ++count;
      }
    }
  }
  return count;
}

///////////////////////////////////////////
// Definitions of Cell stream operations //
///////////////////////////////////////////

std::ostream &operator<<(std::ostream &out, const Cell &cell) {
  out << cell.x << " "
      << cell.y << " "
      << cell.item << " "
      << cell.state << " "
      << cell.has_flag << " "
      << cell.num_adjacent_traps;
  return out;
}

std::istream &operator>>(std::istream &in, Cell &cell) {
  in >> cell.x >> cell.y;
  int item; in >> item; cell.item = static_cast<Item>(item);
  int state; in >> state; cell.state = static_cast<CellState>(state);
  in >> cell.has_flag;
  in >> cell.num_adjacent_traps;
  if (in) {
    assert(item == EMPTY || item == TREASURE || item == TRAP);
  }
  return in;
}