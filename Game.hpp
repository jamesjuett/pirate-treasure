#include <vector>
#include <iostream>
#include <utility>

enum Item {
  EMPTY = 0,
  ONE = 1,
  TWO = 2,
  THREE = 3,
  FOUR = 4,
  FIVE = 5,
  SIX = 6,
  SEVEN = 7,
  EIGHT = 8,
  TREASURE = 9,
  TRAP = 10,
};

bool Item_is_number(Item item);

enum CellState {
  HIDDEN = 0,
  REVEALED = 1,
  FLAG = 2
};

struct Cell {
  Item item;
  CellState state;
};

struct Game {
  int width;
  int height;
  int num_treasures;
  int num_traps;
  int treasures_found;
  std::vector<std::vector<Cell>> cells;
  bool game_over;
  // INVARIANT: items.size() == height
  // INVARIANT: items[r].size() == width for any r
  // INVARIANT: items contains exactly num_treasures TREASUREs
  // INVARIANT: items contains exactly num_traps TRAPs
};

void Game_init(Game* game, int height, int width, int num_treasures, int num_traps);

int Game_width(const Game *game);
int Game_height(const Game *game);
int Game_num_treasures(const Game *game);
int Game_num_traps(const Game *game);

bool Game_in_bounds(const Game* game, int r, int c);

// REQUIRES: 0 <= r < Game_height(game) && 0 <= c < Game_width(game)
Cell * Game_get(Game* game, int r, int c);
const Cell * Game_get(const Game* game, int r, int c);

// REQUIRES: 0 <= r < Game_height(game) && 0 <= c < Game_width(game)
void Game_set(Game* game, int r, int c, Cell cell);

// EFFECTS: Reveals the cell at (r, c), if it was not already revealed.
//          Otherwise, does nothing. If a revealed cell is empty, all
//          adjacent empty cells are also revealed.
void Game_reveal(Game* game, int r, int c);

void Game_mark(Game* game, int r, int c);

void Game_print(const Game* game, bool show_hidden);