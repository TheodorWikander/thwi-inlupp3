#include "hash_table.h"
#include <stdbool.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

//Change this function if you want to do something BEFORE you run a test suite 
int init_suite(void) {
    return 0;
}

//Change this function if you want to do something AFTER you run a test suite
int clean_suite(void) {
    return 0;
}

//Replace with functions of your own

void test_create_destroy() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create();
    CU_ASSERT_PTR_NOT_NULL(ht);
    ioopm_hash_table_destroy(ht);
}

void test_insert_once() {
    //create new ht 
    ioopm_hash_table_t *ht = ioopm_hash_table_create();

    char *key = "abc";
    int value = 123;

    //check that key is not in ht 
    int result = 0; 
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result, 0);

    //insert key-value pair and check that the mapping exists
    ioopm_hash_table_insert(ht, key, value);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result, value);

    //destroy hash table
    ioopm_hash_table_destroy(ht);
}

void test_insert_old_key_new_value() {
    //create new ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create();

    char *key = "abc";
    int value = 123;
    int value2 = 987;
    int result = 0;

    //insert key value pair
    ioopm_hash_table_insert(ht, key, value);

    //use same key and new value
    ioopm_hash_table_insert(ht, key, value2);

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result, value2);

    //destroy hash table
    ioopm_hash_table_destroy(ht);
}

void test_lookup_non_existing_key() {
    //create new ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create();

    char *key = "abc";
    int value = 123;

    //check that key is not in ht
    int result = 0;
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result))
    CU_ASSERT_EQUAL(result, 0);

     //insert key-value pair
    ioopm_hash_table_insert(ht, key, value);

    //Try to lookup a key that is not in ht
    char *fakekey = "xyz";
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, fakekey, &result));
    CU_ASSERT_EQUAL(result, 0);

    //destroy hash table
    ioopm_hash_table_destroy(ht);
}

void test_empty_string_as_key() {
   ioopm_hash_table_t *ht = ioopm_hash_table_create();

   char *key = "";
   int value = 123;

    //check that key is not already in ht
    int result = 0;
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result))
    CU_ASSERT_EQUAL(result, 0);

    //insert key-value pair and check that the mapping exists
    ioopm_hash_table_insert(ht, key, value);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result, value);

    //destroy hash table
    ioopm_hash_table_destroy(ht);
}

void test_updating_value_does_not_affect_other_keys() {
    //create ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create();

    char *key = "abc";
    int value = 123;
    int result = 0;

    //insert key-value pair and check that the mapping exists
    ioopm_hash_table_insert(ht, key, value);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result, value);

    //insert new key-value pair
    char *key2 = "xyz";
    int value2 = 987;
    int result2 = 0;

    ioopm_hash_table_insert(ht, key2, value2);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
    CU_ASSERT_EQUAL(result2, value2);

    //change value at key 1 and check that value is updated
    ioopm_hash_table_insert(ht, key, 321);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result))
    CU_ASSERT_EQUAL(result, 321);

    //check that value at key2 remains the same
    CU_ASSERT_EQUAL(result2, value2);

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_insert_colliding_keys() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create();
    int result = 0;

    ioopm_hash_table_insert(ht, "a", 1);
    ioopm_hash_table_insert(ht, "r", 2); // hamnar i samma bucket då varje nyckel
    ioopm_hash_table_insert(ht, "P", 3); // får sitt ASCII värde % 17 som nyckel
                                         // och detta ger alla värde 12

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "a", &result));
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "r", &result));
    CU_ASSERT_EQUAL(result, 2);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "P", &result));
    CU_ASSERT_EQUAL(result, 3);

    //overwrite vaölue in the middle of the chain
    ioopm_hash_table_insert(ht, "r", 20);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "r", &result));
    CU_ASSERT_EQUAL(result, 20);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "P", &result));
    CU_ASSERT_EQUAL(result, 3);

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) 
        return CU_get_error();

    CU_pSuite my_test_suite = CU_add_suite("hash_table_tests", init_suite, clean_suite);
    if (my_test_suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
if (CU_add_test(my_test_suite, "create destroy", test_create_destroy) == NULL || 
    CU_add_test(my_test_suite, "insert key", test_insert_once) == NULL || 
    CU_add_test(my_test_suite, "update value at key", test_insert_old_key_new_value) == NULL ||
    CU_add_test(my_test_suite, "lookup non-existing key", test_lookup_non_existing_key) == NULL ||
    CU_add_test(my_test_suite, "empty string aas key", test_empty_string_as_key) == NULL ||
    CU_add_test(my_test_suite, "updating value does not affect other keys", test_updating_value_does_not_affect_other_keys) == NULL ||
    CU_add_test(my_test_suite, "collision", test_insert_colliding_keys) == NULL ||
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