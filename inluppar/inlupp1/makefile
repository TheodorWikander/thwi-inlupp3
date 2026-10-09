C_COMPILER = gcc 
C_FLAGS = -Wall -Wextra -g 
CUNIT_LINK = -lcunit

COV_FLAGS = --coverage -O0

TESTS = linked_list_tests list_iterator_tests hash_table_tests iterator_tests

all: $(TESTS)

%.o: %.c
	$(C_COMPILER) $(C_FLAGS) -MMD -MP -c $< -o $@

linked_list_tests: linked_list_tests.o linked_list.o
	$(C_COMPILER) $(C_FLAGS) $^ -o $@ $(CUNIT_LINK)

list_iterator_tests: linked_list.o list_iterator_tests.o list_iterator.o
	$(C_COMPILER) $(C_FLAGS) $^ -o $@ $(CUNIT_LINK)

hash_table_tests: hash_table.o hash_table_tests.o
	$(C_COMPILER) $(C_FLAGS) $^ -o $@ $(CUNIT_LINK)

iterator_tests: hash_table.o iterator.o iterator_tests.o
	$(C_COMPILER) $(C_FLAGS) $^ -o $@ $(CUNIT_LINK)

linked_list_test: linked_list_tests 
	./linked_list_tests

list_iterator_test: list_iterator_tests
	./list_iterator_tests

hash_table_test:
	./hash_table_tests

iterator_test: 
	./iterator_tests

test: hash_table_test iterator_test linked_list_test list_iterator_test

memtest: all
	valgrind --leak-check=full ./linked_list_tests
	valgrind --leak-check=full ./list_iterator_tests
	valgrind --leak-check=full ./hash_table_tests
	valgrind --leak-check=full ./iterator_tests

# ----- COVERAGE -------

coverage-build: C_FLAGS += $(COV_FLAGS)
coverage-build: clean all


coverage: coverage-build test
	@echo "\n--- COVERAGE SUMMARY ---"
	gcovr --root . --exclude '.*test.*' -u
	gcovr --root . --exclude '.*test.*' -u -b
	@echo "Report saved to lcov.info"
	@gcovr --root . --exclude '.*test.*' --lcov lcov.info

clean: 
	rm -f *.o *.gcno *.gcda *.gcov lcov.info $(TESTS)

.PHONY: all test memtest clean coverage coverage-build
