CC := gcc
CXX := g++

TARGET_TETRIS := consolTetris
TARGET_SNAKE := consolSnake
TARGET_SNAKE_DSK := desktopSnake
TARGET_TEST_SNAKE := _snake

RM := rm -rf

CFLAGS := -I inc -std=c11 -Wall -Werror -Wextra
CXXFLAGS := -I inc -std=c++20 -Wall -Werror -Wextra
GCOV_FLAGS := --coverage -lgtest -lsubunit
NCURSES_FLAG := -lncurses

TEST := tests/test
SCORE_FILE := snake_high_score
SNAKE_PRO_FILE := Snake.pro

SRC_BACK := brick_game/tetris/
SRC_GUI := gui/cli/
SRC_BACK_SNAKE := brick_game/snake/

OBJ_BUILD := build/
OBJ_BUILD_CLI := build/cli/
OBJ_BUILD_DSK := build/desktop/

DVI_DIR := dvi/
DIST_DIR := dist/

SRC = $(wildcard $(SRC_BACK)*.c $(SRC_GUI)*.c)
OBJ = $(patsubst $(SRC_BACK)%.c $(SRC_GUI)%.c, $(OBJ_BUILD_CLI)%.o, $(SRC))

SRC_SNAKE = $(wildcard $(SRC_BACK_SNAKE)*.cpp $(SRC_GUI)*.cpp)
OBJ_SNAKE = $(patsubst $(SRC_BACK_SNAKE)%.cpp $(SRC_GUI)%.cpp, $(OBJ_BUILD_CLI)%.o, $(SRC_SNAKE))

TEST_SRC = $(wildcard $(TEST)*.cpp $(SRC_BACK_SNAKE)*.cpp)
TEST_OBJ = $(patsubst $(TEST)%.cpp $(SRC_BACK_SNAKE)*.cpp, $(TEST)%.o, $(TEST_SRC))

RM = rm -rf

all: install

$(OBJ_BUILD_CLI)%.o: $(SRC_BACK)%.c $(SRC_GUI)%.c
	$(CC) $(CFLAGS) -c $< -o $@

install: build clean

build: $(OBJ)
	mkdir -p $(OBJ_BUILD_CLI) $(OBJ_BUILD_DSK)
	cp snake_high_score $(OBJ_BUILD_CLI)
	cp snake_high_score $(OBJ_BUILD_DSK)
	$(CC) $(CFLAGS) $(OBJ) -o $(OBJ_BUILD_CLI)$(TARGET_TETRIS) $(NCURSES_FLAG)
	$(CXX) $(CXXFLAGS) $(OBJ_SNAKE) -o $(OBJ_BUILD_CLI)$(TARGET_SNAKE) $(NCURSES_FLAG)
	cp $(SNAKE_PRO_FILE) $(OBJ_BUILD_DSK)
	cd $(OBJ_BUILD_DSK) && qmake $(SNAKE_PRO_FILE) && make

uninstall:
	$(RM) $(OBJ_BUILD)
	$(RM) $(DVI_DIR)
	$(RM) *.tar.gz

test: clean $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(TEST_OBJ) -o $(TEST)$(TARGET_TEST_SNAKE) $(GCOV_FLAGS)
	./$(TEST)$(TARGET_TEST_SNAKE)

dvi:
	mkdir -p $(DVI_DIR)
	texi2dvi info/info.texi --dvi -o info.dvi
	mv info.* dvi

dist:
	mkdir -p $(DIST_DIR)
	cp -a -r brick_game $(DIST_DIR)
	cp -a -r gui $(DIST_DIR)
	cp -a -r tests $(DIST_DIR)
	cp -a info/info.txt $(DIST_DIR)
	cp -a info/info.texi $(DIST_DIR)
	tar -czf s21_Snake.tar.gz $(DIST_DIR)
	$(RM) $(DIST_DIR)

gcov_report: clean test
	./$(TEST)$(TARGET_TEST_SNAKE) 
	lcov -t  "Tetris_gcov_report" -o report.info -c -d . --ignore-errors mismatch
	genhtml -o gcov_report report.info

clangn:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -n */*/*.[ch]
	clang-format -style=Google -n */*/*.cpp
	rm .clang-format

clangi:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -i */*/*.[ch]
	clang-format -style=Google -i */*/*.cpp
	rm .clang-format
	
mem_leaks: clean test
	valgrind --tool=memcheck --leak-check=yes ./$(TEST)$(TARGET_TEST_SNAKE)

consol_mem: uninstall clean install
	valgrind --tool=memcheck --leak-check=full ./$(OBJ_BUILD_CLI)$(TARGET_SNAKE)

cpp_check:
	cppcheck --enable=all --suppress=missingIncludeSystem */*/*.c

clean:
	$(RM) *.o *.a *.gcda *.gcno *.gcov *.info *.html *.out \
	$(TEST)*.gcda $(TEST)*.gcno $(TEST)*.o $(TEST)*.a $(TEST)$(TARGET_TEST_SNAKE) \
	gcov_report Snake.pro.user
	@if [ -d $(OBJ_BUILD_DSK) ]; then \
		find $(OBJ_BUILD_DSK) -type f ! -name $(TARGET_SNAKE_DSK) \
		! -name snake_high_score -delete; \
	fi

rebuild: uninstall all

.PHONY: all clean rebuild gcov_report install \
 uninstall test dvi clangi clangn dist