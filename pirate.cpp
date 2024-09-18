#include "Game.hpp"
#include "KeyboardUI.hpp"
#include "CommandUI.hpp"
#include <thread>
#include <chrono>
#include <fstream>
#include <string>

// Usage: pirate.exe width height num_treasures num_traps
//   If four arguments are provided, a new game is created with the given parameters.
// Usage: pirate.exe filename
//   If filename is provided, the game state is loaded from the file.

// If the 

int main(int argc, char *argv[]) {

  Game game;

  if (argc == 5) {
    Game_init(&game,
      std::stoi(argv[1]), std::stoi(argv[2]),
      std::stoi(argv[3]), std::stoi(argv[4])
    );
  }
  else if (argc == 2) {
    std::ifstream fin(argv[1]);
    Game_init(&game, fin);
  }
  else {
    // If the user provides the wrong number of arguments, print a usage message.
    std::cerr << "Invalid number of arguments." << std::endl;
    std::cerr << "Usage: " << argv[0] << " width height num_treasures num_traps" << std::endl;
    std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
    return 1;
  }

  #ifdef USE_KEYBOARD_UI
    KeyboardUI keyboard_ui;
    KeyboardUI_init(&keyboard_ui, &game);
    KeyboardUI_play(&keyboard_ui);
  #else
    CommandUI command_ui;
    CommandUI_init(&command_ui, &game);
    CommandUI_play(&command_ui);
  #endif
  
}