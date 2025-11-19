CC := gcc
RM := rm -rf
CFLAGS := -I inc -std=c11 -Wall -Werror -Wextra
GCOV_FLAGS := --coverage -lcheck -lsubunit
NCURSES_FLAG := -lncurses
TEST := tests/test
SRC_BACK := brick_game/tetris/
SRC_GUI := gui/cli/
OBJ_BUILD := build/
DVI_DIR := dvi/
DIST_DIR := dist/

SRC = $(wildcard $(SRC_BACK)*.c $(SRC_GUI)*.c)
OBJ = $(patsubst $(SRC_BACK)%.c $(SRC_GUI)%.c, $(OBJ_BUILD)%.o, $(SRC))

TEST_SRC = $(wildcard $(TEST)*.c $(SRC_BACK)*.c)
TEST_OBJ = $(patsubst $(TEST)%.c $(SRC_BACK)*.c, $(TEST)%.o, $(TEST_SRC))

RM = rm -rf

all: install

$(OBJ_BUILD)%.o: $(SRC_BACK)%.c $(SRC_GUI)%.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(OBJ)
	mkdir -p $(OBJ_BUILD)
	$(CC) $(CFLAGS) $(OBJ) -o $(OBJ_BUILD)tetris $(NCURSES_FLAG)
	./$(OBJ_BUILD)tetris

uninstall:
	$(RM) $(OBJ_BUILD)

test: $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) -o $(TEST)_tetris $(GCOV_FLAGS)
	./$(TEST)_tetris

dvi:
	mkdir -p $(DVI_DIR)
	texi2dvi info.texi --dvi -o info.dvi
	mv info.* dvi
	mv dvi/info.txt .
	mv dvi/info.texi .

dist:
	mkdir -p $(DIST_DIR)
	cp -a -r brick_game $(DIST_DIR)
	cp -a -r gui $(DIST_DIR)
	cp -a -r tests $(DIST_DIR)
	cp -a info.txt $(DIST_DIR)
	cp -a info.texi $(DIST_DIR)
	tar -czf s21_Tetris.tar.gz $(DIST_DIR)
	$(RM) $(DIST_DIR)

gcov_report: clean test
	lcov -t "Tetris_gcov_report" -o report.info -c -d .
	genhtml -o gcov_report report.info

clangn:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -n */*/*.[ch]
	rm .clang-format

clangi:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -i */*/*.[ch]
	rm .clang-format
	
mem_leaks: clean test
	valgrind --tool=memcheck --leak-check=yes ./$(TEST)_tetris

cpp_check:
	cppcheck --enable=all --suppress=missingIncludeSystem */*/*.c

clean:
	$(RM) *.o *.a *.gcda *.gcno *.gcov *.info *.html *.out \
	$(TEST)*.gcda $(TEST)*.gcno $(TEST)*.o $(TEST)*.a $(TEST)_tetris \
	gcov_report *.tar.gz
	$(RM) $(OBJ_BUILD)
	$(RM) $(DVI_DIR)

rebuild: uninstall all

.PHONY: all clean rebuild gcov_report install \
 uninstall test dvi clangi clangn dist