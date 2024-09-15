#ifndef STREAM_UI_HPP
#define STREAM_UI_HPP

#include "Game.hpp"
#include <iostream>


struct StreamUI {
  Game *game;
};

void StreamUI_init(StreamUI *ui, Game *game);

void StreamUI_play(StreamUI* ui);

#endif