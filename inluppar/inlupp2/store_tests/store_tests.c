#include "../store/store.h"
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

void test_create_destroy_store(void) {
    store_t *store = ioopm_store_create();
    CU_ASSERT_PTR_NOT_NULL(store);
    ioopm_store_destroy(store);
}

void test_add_merch(void) {
    store_t *store = ioopm_store_create();

    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "book", "a book", 100));
    CU_ASSERT_EQUAL(ioopm_store_merch_count(store), 1);

    ioopm_store_destroy(store);
}

void test_add_merch_old_name(void) {
    store_t *store = ioopm_store_create();

    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "book", "a book", 100));
    CU_ASSERT_EQUAL(ioopm_store_merch_count(store), 1);
    CU_ASSERT_FALSE(ioopm_store_add_merch(store, "book", "a old book", 200));
    CU_ASSERT_EQUAL(ioopm_store_merch_count(store), 1);

    ioopm_store_destroy(store);
}

void test_has_merch(void) {
    store_t *store = ioopm_store_create();

    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "book", "a book", 100));
    CU_ASSERT_EQUAL(ioopm_store_merch_count(store), 1);
    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "pen", "a pen", 20));
    CU_ASSERT_EQUAL(ioopm_store_merch_count(store), 2);
    CU_ASSERT_TRUE(ioopm_store_has_merch(store, "book"));
    CU_ASSERT_FALSE(ioopm_store_has_merch(store, "paper"));

    ioopm_store_destroy(store);
}

void test_get_merch(void) {
    store_t *store = ioopm_store_create();

    CU_ASSERT_PTR_NULL(ioopm_store_get_merch(store, "book"));
    ioopm_store_add_merch(store, "book", "a book", 100);
    merch_t *m = ioopm_store_get_merch(store, "book");
    CU_ASSERT_STRING_EQUAL(ioopm_merch_name(m), "book");
    CU_ASSERT_STRING_EQUAL(ioopm_merch_description(m), "a book");
    CU_ASSERT_EQUAL(ioopm_merch_price(m), 100);

    ioopm_store_destroy(store);
}

void test_remove_merch(void) {
    store_t *store = ioopm_store_create();

    ioopm_store_add_merch(store, "book", "a book", 100);
    CU_ASSERT_TRUE(ioopm_store_has_merch(store, "book"));
    CU_ASSERT_TRUE(ioopm_store_remove_merch(store, "book"));
    CU_ASSERT_FALSE(ioopm_store_remove_merch(store, "book"));

    ioopm_store_destroy(store);
}

void test_edit_merch(void) {
    store_t *store = ioopm_store_create();
    ioopm_store_add_merch(store, "book", "a book", 100);
    ioopm_store_add_merch(store, "pen", "a pen", 20);

    CU_ASSERT_TRUE(ioopm_store_edit_merch(store, "book", "book", "a new book", 10));
    CU_ASSERT_EQUAL(ioopm_merch_price(ioopm_store_get_merch(store, "book")), 10);

    CU_ASSERT_TRUE(ioopm_store_edit_merch(store, "book", "novel", "a novel", 20));
    CU_ASSERT_TRUE(ioopm_store_has_merch(store, "novel"));
    CU_ASSERT_FALSE(ioopm_store_has_merch(store, "book"));

    CU_ASSERT_FALSE(ioopm_store_edit_merch(store, "novel", "pen", "a pen", 20));
    CU_ASSERT_TRUE(ioopm_store_has_merch(store, "novel"));
    CU_ASSERT_PTR_NOT_NULL_FATAL(ioopm_store_get_merch(store, "novel"));
    CU_ASSERT_EQUAL(ioopm_merch_price(ioopm_store_get_merch(store, "novel")),20);
    
    CU_ASSERT_FALSE(ioopm_store_edit_merch(store, "paper", "clear paper", "a paper", 10));
    CU_ASSERT_FALSE(ioopm_store_has_merch(store, "clear paper"));

    CU_ASSERT_EQUAL(ioopm_store_merch_count(store),2);
    ioopm_store_destroy(store);
}

void test_replenish(void) {
    store_t *store = ioopm_store_create();

    ioopm_store_add_merch(store, "book", "a book", 100);
    CU_ASSERT_EQUAL(ioopm_store_merch_stock(store, "book"), 0);
    CU_ASSERT_TRUE(ioopm_store_replenish(store, "A01", "book", 10));
    CU_ASSERT_EQUAL(ioopm_store_merch_stock(store, "book"), 10);
    CU_ASSERT_TRUE(ioopm_store_replenish(store, "A01", "book", 5));
    CU_ASSERT_EQUAL(ioopm_store_merch_stock(store, "book"), 15);
    CU_ASSERT_TRUE(ioopm_store_replenish(store, "A02", "book", 5));
    CU_ASSERT_EQUAL(ioopm_store_merch_stock(store, "book"), 20);

    //shelf taken by another merch
    CU_ASSERT_FALSE(ioopm_store_replenish(store, "A01", "pen", 5));
    CU_ASSERT_EQUAL(ioopm_store_merch_stock(store, "pen"), 0);

    //invalid amount
    CU_ASSERT_FALSE(ioopm_store_replenish(store, "B01", "book", 0));
    CU_ASSERT_FALSE(ioopm_store_replenish(store, "B01", "book", -1));

    //unknown merch item
    CU_ASSERT_FALSE(ioopm_store_replenish(store, "B01", "pencil", 1));

    //invalid shelfs (shelf-name)
    CU_ASSERT_FALSE(ioopm_store_replenish(store, "a01", "book", 1));
    CU_ASSERT_FALSE(ioopm_store_replenish(store, "A1", "book", 1));
    CU_ASSERT_FALSE(ioopm_store_replenish(store, "AB01", "book", 1));
    CU_ASSERT_FALSE(ioopm_store_replenish(store, "A001", "book", 1));
    CU_ASSERT_FALSE(ioopm_store_replenish(store, "", "book", 1));

    //no invalid case has changed the stock
    CU_ASSERT_EQUAL(ioopm_store_merch_stock(store, "book"), 20);

    ioopm_store_destroy(store);
}






int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) 
        return CU_get_error();

    CU_pSuite my_test_suite = CU_add_suite("store_tests", init_suite, clean_suite);
    if (my_test_suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
if (CU_add_test(my_test_suite, "create destroy", test_create_destroy_store) == NULL ||
    CU_add_test(my_test_suite, "add merch", test_add_merch) == NULL ||
    CU_add_test(my_test_suite, "add merch old name", test_add_merch_old_name) == NULL ||
    CU_add_test(my_test_suite, "has merch", test_has_merch) == NULL ||
    CU_add_test(my_test_suite, "get merch", test_get_merch) == NULL ||
    CU_add_test(my_test_suite, "remove merch", test_remove_merch) == NULL ||
    CU_add_test(my_test_suite, "edit merch", test_edit_merch) == NULL ||
    CU_add_test(my_test_suite, "test replenish", test_replenish) == NULL ||
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