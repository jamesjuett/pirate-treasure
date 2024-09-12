#include "unit_test_framework.hpp"
#include "Game.hpp"

TEST(test_game_init) {
  Game game;
  Game_init(&game, 5, 6, 3, 4);
  ASSERT_EQUAL(Game_width(&game), 5);
  ASSERT_EQUAL(Game_height(&game), 6);
  ASSERT_EQUAL(Game_num_treasures(&game), 3);
  ASSERT_EQUAL(Game_num_traps(&game), 4);
}

TEST_MAIN()