#include <check.h>
#include "backend.h"

void print_matrix(int **field, int y, int x)
{
  for (int i = 0; i < y; i++)
  {
    for (int j = 0; j < x; j++)
    {
      printf("%d ", field[i][j]);
    }
    printf("\n");
  }
}

START_TEST(tetris_1)
{
  GameParams_t *tetris = initGame();
  userInput(Start, tetris);
  ck_assert_int_eq(tetris->stage, SPAWN);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_2)
{
  GameParams_t *tetris = initGame();
  userInput(Start, tetris);
  userInput(Up, tetris);
  ck_assert_int_eq(tetris->stage, SHIFT);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_3)
{
  GameParams_t *tetris = initGame();
  userInput(get_action('\n', false), tetris);
  userInput(Up, tetris);
  userInput(Left, tetris);
  ck_assert_int_eq(tetris->stage, MOVING);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_4)
{
  GameParams_t *tetris = initGame();
  userInput(Start, tetris);
  userInput(Up, tetris);
  tetris->stage = GAMEOVER;
  userInput(Start, tetris);
  ck_assert_int_eq(tetris->stage, START);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_5)
{
  GameParams_t *tetris = initGame();
  for (int j = 0; j < FIELD_X; j++)
    tetris->data->field[FIELD_Y - 1][j] = 1;
  on_attach_stage(tetris);
  ck_assert_int_eq(tetris->data->score, 100);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_6)
{
  GameParams_t *tetris = initGame();
  on_spawn_stage(tetris);
  plantFigure(tetris);
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      ck_assert_int_eq(tetris->figure->blocks[i][j],
                       tetris->data->field[i][j + 3]);
    }
  }
  updateParams(tetris);
  updateCurrentState();
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_7)
{
  GameParams_t *tetris = initGame();
  for (int j = 0; j < 4; j++)
    tetris->figure->blocks[0][j] = 1;
  on_moving_stage(Action, tetris);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(tetris->figure->blocks[i][0], 1);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_8)
{
  GameParams_t *tetris = initGame();
  on_spawn_stage(tetris);
  on_moving_stage(Down, tetris);
  ck_assert_int_eq(tetris->figure->y, 1);
  ck_assert_int_eq(tetris->figure->x, 3);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_9)
{
  GameParams_t *tetris = initGame();
  on_spawn_stage(tetris);
  on_moving_stage(Right, tetris);
  ck_assert_int_eq(tetris->figure->y, 0);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_10)
{
  GameParams_t *tetris = initGame();
  on_spawn_stage(tetris);
  on_moving_stage(Left, tetris);
  ck_assert_int_eq(tetris->figure->y, 0);
  ck_assert_int_eq(tetris->figure->x, 3 - 1);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_11)
{
  GameParams_t *tetris = initGame();
  on_spawn_stage(tetris);
  on_moving_stage(Terminate, tetris);
  ck_assert_int_eq(tetris->stage, EXIT);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_12)
{
  GameParams_t *tetris = initGame();
  on_spawn_stage(tetris);
  on_moving_stage(Pause, tetris);
  ck_assert_int_eq(tetris->stage, PAUSE);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_13)
{
  GameParams_t *tetris = initGame();
  on_spawn_stage(tetris);
  on_moving_stage(Up, tetris);
  ck_assert_int_eq(tetris->stage, SHIFT);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_14)
{
  GameParams_t *tetris = initGame();
  on_pause_stage(Terminate,tetris);
  ck_assert_int_eq(tetris->stage, EXIT);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_15)
{
  GameParams_t *tetris = initGame();
  on_pause_stage(Pause,tetris);
  ck_assert_int_eq(tetris->stage, MOVING);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_16)
{
  GameParams_t *tetris = initGame();
  UserAction_t act = get_action('a', false);
  ck_assert_int_eq(act, Action);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_17)
{
  GameParams_t *tetris = initGame();
  UserAction_t act = get_action(' ', false);
  ck_assert_int_eq(act, Pause);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_18)
{
  GameParams_t *tetris = initGame();
  UserAction_t act = get_action(27, false);
  ck_assert_int_eq(act, Terminate);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_19)
{
  GameParams_t *tetris = initGame();
  UserAction_t act = get_action(KEY_DOWN, false);
  ck_assert_int_eq(act, Down);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_20)
{
  GameParams_t *tetris = initGame();
  UserAction_t act = get_action(KEY_LEFT, false);
  ck_assert_int_eq(act, Left);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_21)
{
  GameParams_t *tetris = initGame();
  UserAction_t act = get_action(KEY_RIGHT, false);
  ck_assert_int_eq(act, Right);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_22)
{
  GameParams_t *tetris = initGame();
  tetris->data->score = 100;
  ck_assert_int_eq(tetris->data->level, 0);
  freeGame(tetris);
}
END_TEST

START_TEST(tetris_23)
{
  GameParams_t *tetris = initGame();
  tetris->data->score = 700;
  tetris->erased_lines = 6;
  score_and_levels(tetris);
  ck_assert_int_eq(tetris->data->level, 1);
  ck_assert_int_eq(tetris->data->speed, 1);
  freeGame(tetris);
}
END_TEST

Suite *test_tetris()
{
  Suite *suit = suite_create("\033[102m-=S21_TETRIS=-\033[0m");
  TCase *tc = tcase_create("Backend");
  tcase_add_test(tc, tetris_1);
  tcase_add_test(tc, tetris_2);
  tcase_add_test(tc, tetris_3);
  tcase_add_test(tc, tetris_4);
  tcase_add_test(tc, tetris_5);
  tcase_add_test(tc, tetris_6);
  tcase_add_test(tc, tetris_7);
  tcase_add_test(tc, tetris_8);
  tcase_add_test(tc, tetris_9);
  tcase_add_test(tc, tetris_10);
  tcase_add_test(tc, tetris_11);
  tcase_add_test(tc, tetris_12);
  tcase_add_test(tc, tetris_13);
  tcase_add_test(tc, tetris_14);
  tcase_add_test(tc, tetris_15);
  tcase_add_test(tc, tetris_16);
  tcase_add_test(tc, tetris_17);
  tcase_add_test(tc, tetris_18);
  tcase_add_test(tc, tetris_19);
  tcase_add_test(tc, tetris_20);
  tcase_add_test(tc, tetris_21);
  tcase_add_test(tc, tetris_22);
  tcase_add_test(tc, tetris_23);
  suite_add_tcase(suit, tc);

  return suit;
}

int main(void)
{
  int failed = 0;
  Suite *s21_BrickGame = test_tetris();
  SRunner *sr = srunner_create(s21_BrickGame);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);

  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  printf("========= FAILED: %d =========\n", failed);

  return failed == 0 ? 0 : 1;
}
