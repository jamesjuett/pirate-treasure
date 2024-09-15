#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <iostream>
#include <utility>

enum Item {
  EMPTY = 0,
  TREASURE = 1,
  TRAP = 2,
};

enum CellState {
  HIDDEN = 0,
  REVEALED = 1,
  FLAG = 2
};

// "Plain Old Data" (POD)
struct Cell {
  int x;
  int y;
  Item item;
  CellState state;
  bool has_flag;
  int num_adjacent_traps;
};

// Allow reading/writing cells to/from streams
std::ostream &operator<<(std::ostream &out, const Cell &cell);
std::istream &operator>>(std::istream &out, Cell &cell);

struct Game {
  int width;
  int height;
  int num_treasures;
  int num_traps;
  int treasures_found;
  bool game_over;

  std::vector<std::vector<Cell>> cells;
  // INVARIANT: cells.size() == width
  // INVARIANT: cells[x].size() == height for any r
  // INVARIANT: cells contains exactly num_treasures TREASUREs
  // INVARIANT: cells contains exactly num_traps TRAPs
};

////////////////////////////////////////////////////////////
// Declarations of "Public Interface" Game ADT Functions. //
////////////////////////////////////////////////////////////

// REQUIRES: width > 0, height > 0
//           num_treasures + num_traps < width * height / 2
// EFFECTS: Initializes a Game with the given width and height. The specified
//          number of treasures and traps are placed in random locations.
void Game_init(Game* game, int width, int height, int num_treasures, int num_traps);

// EFFECTS: returns the width of the game board
int Game_width(const Game *game);

// EFFECTS: returns the height of the game board
int Game_height(const Game *game);

// EFFECTS: returns the number of treasures in the game
int Game_num_treasures(const Game *game);

// EFFECTS: returns the number of traps in the game
int Game_num_traps(const Game *game);

// EFFECTS: Returns true if (x,y) is the position of a valid cell.
bool Game_in_bounds(const Game* game, int x, int y);

// EFFECTS: Returns true if a TRAP has been revealed.
bool Game_is_over(const Game* game);

// REQUIRES: Game_in_bounds(x,y)
// EFFECTS: Returns a pointer to the Cell at (x,y). The Cell may
//          not be modified through the pointer.
const Cell * Game_cell(const Game* game, int x, int y);

// REQUIRES: Game_in_bounds(x,y)
// EFFECTS: Reveals the cell at (x,y), if it was not already revealed.
//          Otherwise, does nothing. If an empty cell is revealed, all
//          adjacent empty cells are also revealed.
void Game_reveal(Game* game, int x, int y);

// REQUIRES: Game_in_bounds(x,y)
// EFFECTS: If the cell at (x,y) has the state HIDDEN, its state is
//          changed to FLAG. If the state was FLAG, it is change to
//          HIDDEN. If the state was REVEALED, nothing happens.
void Game_toggle_flag(Game* game, int x, int y);

#endif