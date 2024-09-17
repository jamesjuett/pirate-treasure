#include "Game.hpp"
#include "TUI.hpp"
#include "StreamUI.hpp"
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

  #ifdef USE_TUI
    TUI tui;
    TUI_init(&tui, &game);
    TUI_play(&tui);
  #else
    StreamUI stream_ui;
    StreamUI_init(&stream_ui, &game);
    StreamUI_play(&stream_ui);
  #endif
  
}