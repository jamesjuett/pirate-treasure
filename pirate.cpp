#include "Game.hpp"
#include "TUI.hpp"
#include "StreamUI.hpp"
#include <thread>
#include <chrono>

int main() {
  Game game;
  Game_init(&game, 15, 10, 25, 25);
  TUI tui;
  TUI_init(&tui, &game);
  TUI_play(&tui);

  // StreamUI stream_ui;
  // StreamUI_init(&stream_ui, &game);

  // while(!Game_is_over(&game)) {
  //   StreamUI_print(&stream_ui, false);
  //   StreamUI_input(&stream_ui);
  // }

}