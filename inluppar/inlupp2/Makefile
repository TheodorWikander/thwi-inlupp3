CC = gcc
CFLAGS = -Wall -Wextra -g -std=gnu11 --coverage
LIBS = -lcunit

STRUCT_DIR = generic_structures
UTILS_DIR = generic_utils
STORE_DIR = store
UI_DIR = ui
TEST_DIR = store_tests


GENERIC =	$(STRUCT_DIR)/hash_table.c \
			$(STRUCT_DIR)/iterator.c \
			$(STRUCT_DIR)/linked_list.c \
			$(STRUCT_DIR)/list_iterator.c \
			$(UTILS_DIR)/utils.c 
LOGIC = 	$(STORE_DIR)/store.c
UI = 		$(UI_DIR)/ui.c

store_app: $(UI_DIR)/main.c $(UI) $(LOGIC) $(GENERIC)
	$(CC) $(CFLAGS) $^ -o $@


tests_bin: $(TEST_DIR)/store_tests.c $(LOGIC) $(GENERIC)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test: tests_bin
	./tests_bin

memtest: tests_bin
	valgrind --leak-check=full --show-leak-kinds=all ./tests_bin

run: store_app
	./store_app

clean: 
	rm -f store_app tests_bin
	find . -name "*.gcda" -o -name "*.gcno" -o -name "*.gcov" | xargs rm -f

.PHONY: test memtest run clean
