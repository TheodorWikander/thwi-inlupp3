#include "linked_list.h"
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

void test_create_destroy_list(void) {
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_PTR_NOT_NULL(list);
    ioopm_list_destroy(list);
}

void test_empty_list(void) {
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));
    CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
    ioopm_list_destroy(list);
}

void test_append_once(void) {
    ioopm_list_t *list = ioopm_list_create();
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));
    CU_ASSERT_EQUAL(ioopm_list_size(list), 0);

    ioopm_list_append(list, 1);
    CU_ASSERT_FALSE(ioopm_list_is_empty(list));
    CU_ASSERT_EQUAL(ioopm_list_size(list), 1);

    ioopm_list_destroy(list);
}

void test_append_multiple(void) {
    ioopm_list_t *list = ioopm_list_create();
    for (int i = 0; i < 5; ++i) {
        ioopm_list_append(list, i);
    }

    CU_ASSERT_EQUAL(ioopm_list_size(list), 5);

    ioopm_list_destroy(list);

}

void test_prepend_once(void) {

    ioopm_list_t *list = ioopm_list_create();

    CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));

    ioopm_list_prepend(list, 1);

    CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
    CU_ASSERT_EQUAL(ioopm_list_head(list), 1);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 1);

    ioopm_list_destroy(list);

}

void test_prepend_multiple(void) {

   ioopm_list_t *list = ioopm_list_create();

   for (int i = 0; i < 5; ++i) {
    ioopm_list_prepend(list, i);
   }

   CU_ASSERT_EQUAL(ioopm_list_size(list), 5);
   CU_ASSERT_EQUAL(ioopm_list_head(list), 4);
   CU_ASSERT_EQUAL(ioopm_list_last(list), 0);

   ioopm_list_destroy(list);

}

void test_append_and_prepend_mixed(void) {
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 2);
    ioopm_list_prepend(list, 1);
    ioopm_list_append(list, 3);

    CU_ASSERT_EQUAL(ioopm_list_head(list), 1);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 3);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);

    ioopm_list_destroy(list);
}

void test_get_from_list_w_one_element(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 1);

    CU_ASSERT_EQUAL(ioopm_list_get(list, 0), 1);

    ioopm_list_destroy(list);
}

void test_get_first_middle_last(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);

    CU_ASSERT_EQUAL(ioopm_list_get(list,0), 1);
    CU_ASSERT_EQUAL(ioopm_list_get(list,1), 2);
    CU_ASSERT_EQUAL(ioopm_list_get(list,2), 3);

    ioopm_list_destroy(list);
}

void test_get_after_list_altered_w_prepend(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 1);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 0), 1);

    ioopm_list_prepend(list, 2);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 0), 2);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 1), 1);

    ioopm_list_destroy(list);
}

void test_insert_into_empty(void) {
    ioopm_list_t *list = ioopm_list_create();

    CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));

    //insert value 1 at index 0 (first after sentinel)
    ioopm_list_insert(list, 0, 1);

    CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
    CU_ASSERT_FALSE(ioopm_list_is_empty(list));
    CU_ASSERT_EQUAL(ioopm_list_head(list), 1);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 1);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 0), 1);

    ioopm_list_destroy(list);
}

void test_insert_at_start(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    ioopm_list_insert(list, 0, 1);

    CU_ASSERT_EQUAL(ioopm_list_get(list, 0), 1);
    CU_ASSERT_EQUAL(ioopm_list_head(list), 1);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 1), 2);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 2), 3);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 3)
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);

    ioopm_list_destroy(list);
}

void test_insert_in_middle(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 1);
    ioopm_list_append(list, 3);
    ioopm_list_insert(list, 1, 2);

    CU_ASSERT_EQUAL(ioopm_list_head(list), 1);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 3);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 1), 2);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 2), 3);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);

    ioopm_list_destroy(list);
}

void test_insert_at_end(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_insert(list, 2, 3);

    CU_ASSERT_EQUAL(ioopm_list_head(list), 1);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 3);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 2), 3);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 1), 2);

    ioopm_list_append(list, 4);

    CU_ASSERT_EQUAL(ioopm_list_get(list, 3), 4);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 4);

    ioopm_list_destroy(list);
}

void test_remove_only_element(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 1);

    CU_ASSERT_FALSE(ioopm_list_is_empty(list));
    CU_ASSERT_EQUAL(ioopm_list_size(list), 1);

    CU_ASSERT_EQUAL(ioopm_list_remove(list, 0), 1);

    CU_ASSERT_TRUE(ioopm_list_is_empty(list));
    CU_ASSERT_EQUAL(ioopm_list_size(list), 0);

    ioopm_list_destroy(list);
}

void test_remove_first(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);

    CU_ASSERT_EQUAL(ioopm_list_remove(list, 0), 1);
    CU_ASSERT_FALSE(ioopm_list_is_empty(list));
    CU_ASSERT_EQUAL(ioopm_list_head(list), 2);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 3);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);

    ioopm_list_destroy(list);
}

void test_remove_middle(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);

    CU_ASSERT_EQUAL(ioopm_list_remove(list, 1), 2);
    CU_ASSERT_FALSE(ioopm_list_is_empty(list));
    CU_ASSERT_EQUAL(ioopm_list_head(list), 1);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 1), 3);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 3);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);

    ioopm_list_destroy(list);
}

void test_remove_last(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);    

    CU_ASSERT_EQUAL(ioopm_list_remove(list, 2), 3);
    CU_ASSERT_FALSE(ioopm_list_is_empty(list));
    CU_ASSERT_EQUAL(ioopm_list_head(list), 1);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 2);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);

    ioopm_list_destroy(list);
}

void test_remove_mul(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3); 

    CU_ASSERT_EQUAL(ioopm_list_remove(list, 0), 1);
    CU_ASSERT_EQUAL(ioopm_list_remove(list, 1), 3);
    CU_ASSERT_EQUAL(ioopm_list_head(list), 2);
    CU_ASSERT_EQUAL(ioopm_list_last(list), 2);
    CU_ASSERT_EQUAL(ioopm_list_get(list, 0), 2);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 1);

    ioopm_list_destroy(list);
}

void test_remove_all(void) {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3); 

    CU_ASSERT_EQUAL(ioopm_list_remove(list, 0), 1);
    CU_ASSERT_EQUAL(ioopm_list_remove(list, 0), 2);
    CU_ASSERT_EQUAL(ioopm_list_remove(list, 0), 3);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
    CU_ASSERT_TRUE(ioopm_list_is_empty(list));

    ioopm_list_destroy(list);
}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) 
        return CU_get_error();

    CU_pSuite my_test_suite = CU_add_suite("linked_list_tests", init_suite, clean_suite);
    if (my_test_suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
if (CU_add_test(my_test_suite, "create destroy", test_create_destroy_list) == NULL || 
    CU_add_test(my_test_suite, "is_list_empty", test_empty_list) == NULL ||
    CU_add_test(my_test_suite, "append once", test_append_once) == NULL ||
    CU_add_test(my_test_suite, "append several times", test_append_multiple) == NULL ||
    CU_add_test(my_test_suite, "prepend once", test_prepend_once) == NULL ||
    CU_add_test(my_test_suite, "prepend several times", test_prepend_multiple) == NULL ||
    CU_add_test(my_test_suite, "prepend and append mixed", test_append_and_prepend_mixed) == NULL ||
    CU_add_test(my_test_suite, "get from list w one element", test_get_from_list_w_one_element) == NULL ||
    CU_add_test(my_test_suite, "get first, middle and lasrt elemnt of list", test_get_first_middle_last) == NULL ||
    CU_add_test(my_test_suite, "get after list has been prepended", test_get_after_list_altered_w_prepend) == NULL ||
    CU_add_test(my_test_suite, "insert into empty list", test_insert_into_empty) == NULL ||
    CU_add_test(my_test_suite, "insert at start", test_insert_at_start) == NULL ||
    CU_add_test(my_test_suite, "insert in the middle", test_insert_in_middle) == NULL ||
    CU_add_test(my_test_suite, "insert at end", test_insert_at_end) == NULL ||
    CU_add_test(my_test_suite, "remove only element", test_remove_only_element) == NULL ||
    CU_add_test(my_test_suite, "remove first element", test_remove_first) == NULL ||
    CU_add_test(my_test_suite, "remove middle element", test_remove_middle) == NULL ||
    CU_add_test(my_test_suite, "remove last element", test_remove_last) == NULL ||
    CU_add_test(my_test_suite, "remove multiple elemets", test_remove_mul) == NULL ||
    CU_add_test(my_test_suite, "remove all element", test_remove_all) == NULL ||
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