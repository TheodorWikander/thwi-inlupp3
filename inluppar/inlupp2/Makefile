CC = gcc
CFLAGS = -Wall -Wextra -g -std=gnu11 --coverage
LIBS = -lcunit
SCRIPT = webstoretest.txt

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

stdrun: store_app
	./store_app < $(SCRIPT)

stdmemtest: store_app
	valgrind --leak-check=full --show-leak-kinds=all ./store_app < $(SCRIPT)

clean: 
	rm -f store_app tests_bin
	find . \( -name "*.gcda" -o -name "*.gcno" -o -name "*.gcov" \) -delete

coverage: clean tests_bin
	./tests_bin > /dev/null
	@for f in tests_bin-*.gcda; do gcov -b $$f; done | grep -A3 -E "^File '[a-z_]+/[a-z_]+\.c'"
.PHONY: test memtest run stdrun stdmemtest coverage clean
