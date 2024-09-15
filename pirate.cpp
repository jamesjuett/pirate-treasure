#include "Game.hpp"
#include "TUI.hpp"
#include "StreamUI.hpp"
#include <thread>
#include <chrono>
#include <fstream>

int main() {
  Game game;
  // Game_init(&game, 15, 10, 25, 25);
  std::ifstream fin("game.txt");
  Game_init(&game, fin);
  // TUI tui;
  // TUI_init(&tui, &game);
  // TUI_play(&tui);

  StreamUI stream_ui;
  StreamUI_init(&stream_ui, &game);
  StreamUI_play(&stream_ui);

  // while(!Game_is_over(&game)) {
  //   StreamUI_print(&stream_ui, false);
  //   StreamUI_input(&stream_ui);
  // }

}