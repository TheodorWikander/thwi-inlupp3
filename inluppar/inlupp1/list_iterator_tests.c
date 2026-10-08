#include "list_iterator.h"
#include "linked_list.h"
#include "linked_list_struct.h"
#include <stdbool.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <string.h>

//Change this function if you want to do something BEFORE you run a test suite 
int init_suite(void) {
    return 0;
}

//Change this function if you want to do something AFTER you run a test suite
int clean_suite(void) {
    return 0;
}

void test_iterate_over_empty_list(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list);
}

void test_iterate_over_list_w_one_element(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, int_elem(1));

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(it));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 1);

    ioopm_list_iterator_advance(it);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list);
}

void test_iterate_all_in_order(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, string_elem("a"));
    ioopm_list_append(list, bool_elem(true));

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);
    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(it));

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 1);
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).s, "a");
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).b, true);
    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(it));
    ioopm_list_iterator_advance(it);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list);
}

void test_insert_one_elem_into_empty(void) {
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_insert(it, int_elem(1));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 1);
    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_advance(it);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list);
}

void test_insert_multiple_elem_into_empty(void) {
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));
    ioopm_list_iterator_insert(it, int_elem(1));
    ioopm_list_iterator_insert(it, string_elem("a"));
    ioopm_list_iterator_insert(it, bool_elem(true));

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).b, true);
    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(it));
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).s, "a");
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 1);
    ioopm_list_iterator_advance(it);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list);
}

void test_insert_one_into_non_empty_list(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list,int_elem(1));
    ioopm_list_append(list, int_elem(3));

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(it));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 1);

    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 3);

    ioopm_list_iterator_insert(it, int_elem(2));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 2);

    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 3);

    ioopm_list_iterator_advance(it);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list);
}

void test_insert_at_end_of_non_empty(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, int_elem(1));

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);
    ioopm_list_iterator_advance(it);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));
    ioopm_list_iterator_insert(it, string_elem("a"));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).s, "a");
    ioopm_list_iterator_advance(it);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_destroy(it);

    CU_ASSERT_EQUAL(ioopm_list_last(list).s, "a");
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);

    ioopm_list_destroy(list);
}

void test_inser_mul_into_non_empty(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, int_elem(1));
    ioopm_list_append(list, int_elem(3));
    ioopm_list_append(list, int_elem(5));

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);
    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(it));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 1);

    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 3);

    ioopm_list_iterator_insert(it, string_elem("a"));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).s, "a");
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 3);
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 5);
    ioopm_list_iterator_insert(it, string_elem("b"));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).s, "b");
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, 5);
    ioopm_list_iterator_advance(it);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list);

}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) 
        return CU_get_error();

    CU_pSuite my_test_suite = CU_add_suite("iterator_tests", init_suite, clean_suite);
    if (my_test_suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
if (CU_add_test(my_test_suite, "iterate over empty ht", test_iterate_over_empty_list) == NULL ||
    CU_add_test(my_test_suite, "iterate oer array with one entry", test_iterate_over_list_w_one_element) == NULL ||
    CU_add_test(my_test_suite, "iterate over ht multiple entries", test_iterate_all_in_order) == NULL ||
    CU_add_test(my_test_suite, "insert one element into empty list", test_insert_one_elem_into_empty) == NULL ||
    CU_add_test(my_test_suite, "insert multiple elements into empty list", test_insert_multiple_elem_into_empty) == NULL ||
    CU_add_test(my_test_suite, "insert one element into non empty list", test_insert_one_into_non_empty_list) == NULL ||
    CU_add_test(my_test_suite, "inser multiple lements into non empty list", test_inser_mul_into_non_empty) == NULL ||
    CU_add_test(my_test_suite, "insert at end of non empty list", test_insert_at_end_of_non_empty) == NULL ||
    0) 
    
    {
    CU_cleanup_registry();
    return CU_get_error();
}

CU_basic_set_mode(CU_BRM_VERBOSE);

//run tests
CU_basic_run_tests();

//Tear down CUnit before exiting
CU_cleanup_registry();
return CU_get_error();
} 

