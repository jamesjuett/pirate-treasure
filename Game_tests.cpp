#include "unit_test_framework.hpp"
#include "Game.hpp"

TEST(test_game_init) {
  Game game;
  Game_init(&game, 5, 6, 3, 4);
  ASSERT_EQUAL(Game_width(&game), 5);
  ASSERT_EQUAL(Game_height(&game), 6);

  ASSERT_EQUAL(Game_num_treasures(&game), 3);
  ASSERT_EQUAL(Game_num_traps(&game), 4);
  ASSERT_EQUAL(Game_num_treasures_found(&game), 0);
  ASSERT_EQUAL(Game_num_traps_found(&game), 0);
  ASSERT_FALSE(Game_is_over(&game));
  
  int num_treasures = 0;
  int num_traps = 0;
  for(int x = 0; x < Game_width(&game); ++x) {
    for(int y = 0; y < Game_height(&game); ++y) {
      // check properties of each cell
      ASSERT_EQUAL(Game_cell(&game, x, y)->x, x);
      ASSERT_EQUAL(Game_cell(&game, x, y)->y, y);
      ASSERT_EQUAL(Game_cell(&game, x, y)->state, HIDDEN);
      ASSERT_FALSE(Game_cell(&game, x, y)->has_flag);

      // each cell must be either EMPTY, TREASURE, or TRAP
      ASSERT_TRUE(Game_cell(&game, x, y)->item == EMPTY ||
                  Game_cell(&game, x, y)->item == TREASURE ||
                  Game_cell(&game, x, y)->item == TRAP);

      if(Game_cell(&game, x, y)->item == TREASURE) {
        ++num_treasures;
      }
      else if(Game_cell(&game, x, y)->item == TRAP) {
        ++num_traps;
      }
    }
  }

  ASSERT_EQUAL(num_treasures, Game_num_treasures(&game));
  ASSERT_EQUAL(num_traps, Game_num_traps(&game));

}

TEST(test_game_no_traps) {
  Game game;
  Game_init(&game, 5, 6, 1, 0);
  ASSERT_EQUAL(Game_width(&game), 5);
  ASSERT_EQUAL(Game_height(&game), 6);

  ASSERT_EQUAL(Game_num_treasures(&game), 1);
  ASSERT_EQUAL(Game_num_traps(&game), 0);
  ASSERT_EQUAL(Game_num_treasures_found(&game), 0);
  ASSERT_EQUAL(Game_num_traps_found(&game), 0);
  ASSERT_FALSE(Game_is_over(&game));

  int num_treasures = 0;
  for(int x = 0; x < Game_width(&game); ++x) {
    for(int y = 0; y < Game_height(&game); ++y) {
      if (Game_cell(&game, x, y)->item == TREASURE) {
        ++num_treasures;
      }
      else {
        ASSERT_EQUAL(Game_cell(&game, x, y)->item, EMPTY);
      }
      ASSERT_EQUAL(Game_cell(&game, x, y)->state, HIDDEN);
      ASSERT_EQUAL(Game_cell(&game, x, y)->num_adjacent_traps, 0);
    }
  }
  ASSERT_EQUAL(num_treasures, 1);
}

TEST(test_game_bounds) {
  Game game;
  Game_init(&game, 5, 6, 3, 4);
  ASSERT_EQUAL(Game_width(&game), 5);
  ASSERT_EQUAL(Game_height(&game), 6);

  ASSERT_TRUE(Game_in_bounds(&game, 0, 0));
  ASSERT_TRUE(Game_in_bounds(&game, 4, 5));
  ASSERT_FALSE(Game_in_bounds(&game, 5, 4));
  ASSERT_FALSE(Game_in_bounds(&game, -1, 0));
  ASSERT_FALSE(Game_in_bounds(&game, 0, -1));
  ASSERT_FALSE(Game_in_bounds(&game, -1, -1));
  ASSERT_FALSE(Game_in_bounds(&game, 5, 0));
  ASSERT_FALSE(Game_in_bounds(&game, 0, 6));
}

TEST_MAIN()