# pirate-treasure

A pirate-themed game of logical deduction.

![Pirate Treasure](./pirate-treasure.png)

## How to Play

The game is played on a grid. Some cells are empty while others contain treasures or traps. The contents of cells are initially hidden. Reveal all treasures while avoiding any traps to win the game. If you suspect a trap, mark it with a flag.

### Running the Game

First, compile with `make pirate.exe`.

Then, run with `./pirate.exe` and desired arguments.

For example, to start a new game with the desired width, height, number of treasures, and number of traps:

```console
./pirate.exe <width> <height> <num_treasures> <num_traps>
```

Or, to load a saved game from a file:

```console
./pirate.exe <filename>
```

### Command Interface

The default game interface reads commands from `stdin`:

- `R <x> <y>`: Reveal the contents of the cell at position (x, y).
- `F <x> <y>`: Toggle the flag marker at position (x, y).
- `S <filename>`: Save the current game to a file. 
- `Q`: Quit the game.

### Keyboard Interface

The game also provides an alternate keyboard interface. Compile with `make pirate-keyboard.exe` and run with `./pirate-keyboard.exe` (and desired arguments, as described above).

- `Arrow keys`: Move the cursor.
- `Space`: Reveal the contents of the cell at the cursor.
- `F`: Toggle the flag marker at the cursor.
- `Q`: Quit the game.

(Saving via the keyboard interface is not currently supported.)
