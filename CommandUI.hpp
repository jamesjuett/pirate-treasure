#ifndef STREAM_UI_HPP
#define STREAM_UI_HPP

#include "Game.hpp"
#include <iostream>


struct CommandUI {
  Game *game;
};

void CommandUI_init(CommandUI *ui, Game *game);

void CommandUI_play(CommandUI* ui);

#endif