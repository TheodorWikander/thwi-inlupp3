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
    
    CU_ASSERT_FALSE(ioopm_store_add_merch(store, "bad", "negative price", -5));
    CU_ASSERT_FALSE(ioopm_store_has_merch(store, "bad"));
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
    
    CU_ASSERT_FALSE(ioopm_store_edit_merch(store, "novel", "novel", "a novel", -1));
    CU_ASSERT_EQUAL(ioopm_merch_price(ioopm_store_get_merch(store, "novel")), 20);  

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

void test_show_stock(void) {
    store_t *store = ioopm_store_create();
    ioopm_store_add_merch(store, "book", "a book", 100);
    ioopm_store_add_merch(store, "pen", "a pen", 20);

    //no shelves yet and unknown merch
    CU_ASSERT_EQUAL(ioopm_store_shelf_count(store, "book"), 0);
    CU_ASSERT_EQUAL(ioopm_store_shelf_count(store, "ghost"), 0);

    //insert in "wrong" order
    ioopm_store_replenish(store, "C10", "book", 4);
    ioopm_store_replenish(store, "A20", "book", 1);
    ioopm_store_replenish(store, "B01", "book", 2);
    ioopm_store_replenish(store, "C01", "book", 3);
    ioopm_store_replenish(store, "C01", "book", 3); //total 6

    CU_ASSERT_EQUAL(ioopm_store_shelf_count(store, "book"), 4);

    CU_ASSERT_STRING_EQUAL(ioopm_store_shelf_name(store, "book", 0), "A20");
    CU_ASSERT_STRING_EQUAL(ioopm_store_shelf_name(store, "book", 1), "B01");
    CU_ASSERT_STRING_EQUAL(ioopm_store_shelf_name(store, "book", 2), "C01"); 
    CU_ASSERT_STRING_EQUAL(ioopm_store_shelf_name(store, "book", 3), "C10");

    CU_ASSERT_EQUAL(ioopm_store_shelf_quantity(store, "book", 0), 1);
    CU_ASSERT_EQUAL(ioopm_store_shelf_quantity(store, "book", 1), 2);
    CU_ASSERT_EQUAL(ioopm_store_shelf_quantity(store, "book", 2), 6);
    CU_ASSERT_EQUAL(ioopm_store_shelf_quantity(store, "book", 3), 4);

    CU_ASSERT_EQUAL(ioopm_store_shelf_count(store, "pen"), 0);

    ioopm_store_destroy(store);
}

void test_list_merch(void) {
    store_t *store = ioopm_store_create();

    CU_ASSERT_EQUAL(ioopm_store_merch_count(store), 0);

    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "pen", "a pen", 20));
    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "book", "a book", 100));
    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "cup", "a cup", 50));

    CU_ASSERT_STRING_EQUAL(ioopm_store_merch_name_at(store, 0), "book");
    CU_ASSERT_STRING_EQUAL(ioopm_store_merch_name_at(store, 1), "cup");
    CU_ASSERT_STRING_EQUAL(ioopm_store_merch_name_at(store, 2), "pen");

    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "apple", "an apple", 5));

    CU_ASSERT_STRING_EQUAL(ioopm_store_merch_name_at(store, 0), "apple");
    CU_ASSERT_STRING_EQUAL(ioopm_store_merch_name_at(store, 1), "book");
    CU_ASSERT_STRING_EQUAL(ioopm_store_merch_name_at(store, 2), "cup");
    CU_ASSERT_STRING_EQUAL(ioopm_store_merch_name_at(store, 3), "pen");

    CU_ASSERT_TRUE(ioopm_store_remove_merch(store, "book"));

    CU_ASSERT_STRING_EQUAL(ioopm_store_merch_name_at(store, 0), "apple");
    CU_ASSERT_STRING_EQUAL(ioopm_store_merch_name_at(store, 1), "cup");
    CU_ASSERT_STRING_EQUAL(ioopm_store_merch_name_at(store, 2), "pen");

    ioopm_store_destroy(store);
}

void test_cart(void) {
    store_t *store = ioopm_store_create();

    CU_ASSERT_EQUAL(ioopm_store_cart_create(store), 1);
    CU_ASSERT_EQUAL(ioopm_store_cart_create(store), 2);

    CU_ASSERT_TRUE(ioopm_store_has_cart(store, 1));
    CU_ASSERT_TRUE(ioopm_store_has_cart(store, 2));
    CU_ASSERT_FALSE(ioopm_store_has_cart(store, 10));

    CU_ASSERT_TRUE(ioopm_store_cart_remove(store, 1));
    CU_ASSERT_FALSE(ioopm_store_has_cart(store, 1));
    CU_ASSERT_TRUE(ioopm_store_has_cart(store, 2));

    CU_ASSERT_FALSE(ioopm_store_cart_remove(store, 1));

    CU_ASSERT_EQUAL(ioopm_store_cart_create(store), 3);

    ioopm_store_destroy(store);
}

void test_cart_add(void) {
    store_t *store = ioopm_store_create();

    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "book", "a book", 100));
    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "pen", "a pen", 20));

    CU_ASSERT_TRUE(ioopm_store_replenish(store, "A01", "book", 10));

    CU_ASSERT_EQUAL(ioopm_store_cart_create(store), 1);
    CU_ASSERT_EQUAL(ioopm_store_cart_create(store), 2);

    CU_ASSERT_FALSE(ioopm_store_cart_add(store, 1, "pen", 2));
    CU_ASSERT_TRUE(ioopm_store_cart_add(store, 1, "book", 4));
    CU_ASSERT_EQUAL(ioopm_store_cart_item_count(store, 1, "book"), 4);
    CU_ASSERT_TRUE(ioopm_store_cart_add(store, 1, "book", 2));
    CU_ASSERT_EQUAL(ioopm_store_cart_item_count(store, 1, "book"), 6);

    CU_ASSERT_FALSE(ioopm_store_cart_add(store, 2, "book", 5));
    CU_ASSERT_TRUE(ioopm_store_cart_add(store, 2, "book", 4));

    CU_ASSERT_TRUE(ioopm_store_replenish(store, "B01", "pen", 10));

    //non-existing cart, non-existing merch, amount <= 0 
    CU_ASSERT_FALSE(ioopm_store_cart_add(store, 10, "pen", 5));
    CU_ASSERT_FALSE(ioopm_store_cart_add(store, 1, "pencil", 5));
    CU_ASSERT_FALSE(ioopm_store_cart_add(store, 1, "pen", 0));
    CU_ASSERT_FALSE(ioopm_store_cart_add(store, 1, "pen", -1));

    CU_ASSERT_TRUE(ioopm_store_cart_remove_item(store, 1, "book", 2));
    CU_ASSERT_EQUAL(ioopm_store_cart_item_count(store, 1, "book"), 4);

    CU_ASSERT_TRUE(ioopm_store_cart_add(store, 1, "pen", 2));

    CU_ASSERT_EQUAL(ioopm_store_cart_cost(store, 1), 440);

    CU_ASSERT_EQUAL(ioopm_store_cart_create(store), 3);
    CU_ASSERT_EQUAL(ioopm_store_cart_cost(store, 3), 0);

    CU_ASSERT_TRUE(ioopm_store_cart_remove_item(store, 1, "pen", 2));
    CU_ASSERT_EQUAL(ioopm_store_cart_item_count(store, 1, "pen"), 0);

    ioopm_store_destroy(store);    

}

void test_checkout_cart(void) {
    store_t *store = ioopm_store_create();
    
    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "book", "a book", 100));
    CU_ASSERT_TRUE(ioopm_store_replenish(store, "A01", "book", 5));
    CU_ASSERT_TRUE(ioopm_store_replenish(store, "B02", "book", 10));
    CU_ASSERT_EQUAL(ioopm_store_cart_create(store), 1);
    CU_ASSERT_TRUE(ioopm_store_cart_add(store, 1, "book", 12));
    CU_ASSERT_TRUE(ioopm_store_checkout(store, 1));
    CU_ASSERT_EQUAL(ioopm_store_merch_stock(store, "book"), 3);
    CU_ASSERT_FALSE(ioopm_store_has_cart(store, 1));
    CU_ASSERT_EQUAL(ioopm_store_shelf_count(store, "book"), 1);
    CU_ASSERT_STRING_EQUAL(ioopm_store_shelf_name(store, "book", 0), "B02");
    CU_ASSERT_EQUAL(ioopm_store_shelf_quantity(store, "book", 0), 3);

    CU_ASSERT_TRUE(ioopm_store_add_merch(store, "pen", "a pen", 20));
    CU_ASSERT_TRUE(ioopm_store_replenish(store, "A01", "pen", 5));
    CU_ASSERT_FALSE(ioopm_store_checkout(store, 3));
    CU_ASSERT_EQUAL(ioopm_store_cart_create(store), 2);
    CU_ASSERT_TRUE(ioopm_store_checkout(store, 2));

    ioopm_store_destroy(store);

}

void test_remove_merch_in_cart(void) {
    store_t *store = ioopm_store_create();

    ioopm_store_add_merch(store, "book", "a book", 100);
    ioopm_store_add_merch(store, "pen", "a pen", 20);
    ioopm_store_replenish(store, "A01", "book", 10);
    ioopm_store_replenish(store, "B01", "pen", 10);

    int cart = ioopm_store_cart_create(store);
    ioopm_store_cart_add(store, cart, "book", 4);
    ioopm_store_cart_add(store, cart, "pen", 2);
    CU_ASSERT_EQUAL(ioopm_store_cart_cost(store, cart), 440);

    CU_ASSERT_TRUE(ioopm_store_remove_merch(store, "book"));

    CU_ASSERT_EQUAL(ioopm_store_cart_item_count(store, cart, "book"), 0);
    CU_ASSERT_EQUAL(ioopm_store_cart_item_count(store, cart, "pen"), 2);
    CU_ASSERT_EQUAL(ioopm_store_cart_cost(store, cart), 40);   // bara pennorna kvar
    CU_ASSERT_TRUE(ioopm_store_checkout(store, cart));         // får inte krascha
    CU_ASSERT_EQUAL(ioopm_store_merch_stock(store, "pen"), 8);

    ioopm_store_destroy(store);
}

void test_edit_merch_in_cart(void) {
    store_t *store = ioopm_store_create();

    ioopm_store_add_merch(store, "book", "a book", 100);
    ioopm_store_replenish(store, "A01", "book", 10);
    int cart = ioopm_store_cart_create(store);
    ioopm_store_cart_add(store, cart, "book", 4);

    CU_ASSERT_TRUE(ioopm_store_edit_merch(store, "book", "novel", "a novel", 50));

    CU_ASSERT_EQUAL(ioopm_store_cart_item_count(store, cart, "book"), 0);
    CU_ASSERT_EQUAL(ioopm_store_cart_item_count(store, cart, "novel"), 4);
    CU_ASSERT_EQUAL(ioopm_store_cart_cost(store, cart), 200);  
    CU_ASSERT_TRUE(ioopm_store_checkout(store, cart));
    CU_ASSERT_EQUAL(ioopm_store_merch_stock(store, "novel"), 6);

    ioopm_store_destroy(store);
}

void test_cart_remove_item_errors(void) {
    store_t *store = ioopm_store_create();
    ioopm_store_add_merch(store, "book", "a book", 100);
    ioopm_store_replenish(store, "A01", "book", 10);
    int cart = ioopm_store_cart_create(store);
    ioopm_store_cart_add(store, cart, "book", 3);

    CU_ASSERT_FALSE(ioopm_store_cart_remove_item(store, 99, "book", 1));   
    CU_ASSERT_FALSE(ioopm_store_cart_remove_item(store, cart, "pen", 1));   
    CU_ASSERT_FALSE(ioopm_store_cart_remove_item(store, cart, "book", 4));  
    CU_ASSERT_FALSE(ioopm_store_cart_remove_item(store, cart, "book", -1)); 
    CU_ASSERT_EQUAL(ioopm_store_cart_item_count(store, cart, "book"), 3);   

    CU_ASSERT_TRUE(ioopm_store_cart_remove_item(store, cart, "book", 0));   
    CU_ASSERT_EQUAL(ioopm_store_cart_item_count(store, cart, "book"), 3);

    CU_ASSERT_TRUE(ioopm_store_cart_remove_item(store, cart, "book", 3));   
    CU_ASSERT_EQUAL(ioopm_store_cart_item_count(store, cart, "book"), 0);

    ioopm_store_destroy(store);
}

//simple test to raise coverage on utils.c file
void test_utils_hash(void) {
    char a[] = "book";
    char b[] = "book";          

    CU_ASSERT_TRUE(string_eq(string_elem(a), string_elem(b)));
    CU_ASSERT_EQUAL(string_hash(string_elem(a)), string_hash(string_elem(b)));
    CU_ASSERT_FALSE(string_eq(string_elem(a), string_elem("pen")));

    CU_ASSERT_TRUE(int_eq(int_elem(3), int_elem(3)));
    CU_ASSERT_FALSE(int_eq(int_elem(3), int_elem(4)));

    CU_ASSERT_TRUE(is_number("42"));
    CU_ASSERT_TRUE(is_number("-7"));
    CU_ASSERT_FALSE(is_number(""));
    CU_ASSERT_FALSE(is_number("-"));
    CU_ASSERT_FALSE(is_number("4a"));
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
    CU_add_test(my_test_suite, "show stock", test_show_stock) == NULL ||
    CU_add_test(my_test_suite, "list merch", test_list_merch) == NULL ||
    CU_add_test(my_test_suite, "carts", test_cart) == NULL ||
    CU_add_test(my_test_suite, "cart add/remove", test_cart_add) == NULL ||
    CU_add_test(my_test_suite, "checkout", test_checkout_cart) == NULL ||
    CU_add_test(my_test_suite, "remove merch in cart", test_remove_merch_in_cart) == NULL ||
    CU_add_test(my_test_suite, "edit in cart", test_edit_merch_in_cart) ==  NULL ||
    CU_add_test(my_test_suite, "cart remove item errors", test_cart_remove_item_errors) == NULL ||
    CU_add_test(my_test_suite, "utils hash", test_utils_hash) == NULL ||
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