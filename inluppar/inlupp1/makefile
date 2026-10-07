C_COMPILER = gcc
C_FLAGS = -Wall -Wextra -g
CUNIT_LINK = -lcunit

all: linked_list_tests list_iterator_tests

%.o: %.c
	$(C_COMPILER) $(C_FLAGS) -c $<

linked_list.o linked_list_tests.o: linked_list.h linked_list_struct.h
list_iterator.o list_iterator_tests.o: linked_list.h list_iterator.h linked_list_struct.h

linked_list_tests: linked_list_tests.o linked_list.o
	$(C_COMPILER) $(C_FLAGS) $^ -o $@ $(CUNIT_LINK)

list_iterator_tests: linked_list.o list_iterator_tests.o list_iterator.o
	$(C_COMPILER) $(C_FLAGS) $^ -o $@ $(CUNIT_LINK)

linked_list_test: linked_list_tests 
	./linked_list_tests

list_iterator_test: list_iterator_tests
	./list_iterator_tests

memtest: linked_list_tests
	valgrind --leak-check=full ./linked_list_tests
	valgrind --leak-check=full ./list_iterator_tests

clean: 
	rm -f *.o linked_list_tests

.PHONY: all test memtest clean
