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
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
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
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
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
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result, 321);

    //check that value at key2 remains the same
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
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

    //overwrite value in the middle of the chain
    ioopm_hash_table_insert(ht, "r", 20);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "r", &result));
    CU_ASSERT_EQUAL(result, 20);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "P", &result));
    CU_ASSERT_EQUAL(result, 3);

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_remove_one_value_existing_key() {
    //create new ht 
    ioopm_hash_table_t *ht = ioopm_hash_table_create();

    char *key = "abc";
    int value = 123;
    int result = 0; 
    int removed_value = 0;

    //insert key-value pair and check that the mapping exists
    ioopm_hash_table_insert(ht, key, value);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result, value);

    //remove value stored at key and save the removed value in the varable removed_value
    //Check that the value removed is the value we intended. 
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key, &removed_value));
    CU_ASSERT_EQUAL(removed_value, value);

    //COntroll that value is no longer in ht
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
    
    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_remove_value_empty_ht() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create();

    char *key = "abc";
    int removed_value = 0;

    CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, key, &removed_value));
    CU_ASSERT_EQUAL(removed_value, 0);

    ioopm_hash_table_destroy(ht);
}

void test_remove_value_non_existing_key() {
     //create new ht 
    ioopm_hash_table_t *ht = ioopm_hash_table_create();

    char *key = "abc";
    int value = 123;
    char *key2 = "xyz";
    int value2 = 987;
    char *key3 = "i";
    int result = 0; 
    int result2 = 0; 
    int removed_value = 0;

    //insert key-value pairs and check that the mappings exists
    ioopm_hash_table_insert(ht, key, value);
    ioopm_hash_table_insert(ht, key2, value2);

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result, value);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
    CU_ASSERT_EQUAL(result2, value2);

    //try to remove a value at a non existing key. 
    CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, key3, &removed_value));
    CU_ASSERT_EQUAL(removed_value, 0);

    //check that the other mappins are intact after failed remove
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result, value);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
    CU_ASSERT_EQUAL(result2, value2);

    //remove ht
    ioopm_hash_table_destroy(ht);
}

void test_remove_value_twice() {
     //create new ht 
    ioopm_hash_table_t *ht = ioopm_hash_table_create();

    char *key = "abc";
    int value = 123;
    int result = 0;
    int removed_value = 0;

    //insert pair and check that mapping exists and is correct
    ioopm_hash_table_insert(ht, key, value);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(value, result);

    //remove value and check that the value removed was the right one
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key, &removed_value));
    CU_ASSERT_EQUAL(removed_value, value);

    //Double check value is not in ht
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

    //try to remove the same value again and check that it does not work
    CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, key, &removed_value));

    //controll that removed value has not changed when remove failed
    CU_ASSERT_EQUAL(removed_value, 123);

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_removing_one_value_not_affecting_other() {
    //create new ht 
    ioopm_hash_table_t *ht = ioopm_hash_table_create();

    char *key = "abc";
    int value = 123;
    char *key2 = "xyz";
    int value2 = 987;
    int result = 0; 
    int result2 = 0; 
    int removed_value = 0;

    //insert key-value pairs and check that the mappings exists
    ioopm_hash_table_insert(ht, key, value);
    ioopm_hash_table_insert(ht, key2, value2);

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result, value);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
    CU_ASSERT_EQUAL(result2, value2);

    //remove one value and check that the correct value was removed
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key, &removed_value));
    CU_ASSERT_EQUAL(removed_value, value);

    //check that the removed value is actualy removed
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

    //check that the other pair remain intact still
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
    CU_ASSERT_EQUAL(result2, value2);

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_remove_one_value_in_bucket_w_mul_values() {
    //create new ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create();
    
    int result = 0;
    int removed_value = 0;
    
    //insert 3 separate pairs in the same bucket  (hashfunc % 17 puts them all in bucket 12)
    ioopm_hash_table_insert(ht, "a", 1);
    ioopm_hash_table_insert(ht, "r", 2);
    ioopm_hash_table_insert(ht, "P", 3); 

    //Check each pair was inserted correctly
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "a", &result));
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "r", &result));
    CU_ASSERT_EQUAL(result, 2);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "P", &result));
    CU_ASSERT_EQUAL(result, 3);

    //remove value in the middle
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, "r", &removed_value));

    //check that the right value was removed and is not still in ht
    CU_ASSERT_EQUAL(removed_value, 2);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, "r", &result));

    //check that the other pairs are still intact
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "a", &result));
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "P", &result));
    CU_ASSERT_EQUAL(result, 3);

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

// Fler test som kan skapas:
// Ta bort första och sista noden i en kedja
// Infoga ta bort och infoga igen med samma nyckel
// Många nycklar

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
    CU_add_test(my_test_suite, "empty string as key", test_empty_string_as_key) == NULL ||
    CU_add_test(my_test_suite, "updating value does not affect other keys", test_updating_value_does_not_affect_other_keys) == NULL ||
    CU_add_test(my_test_suite, "collision", test_insert_colliding_keys) == NULL ||
    CU_add_test(my_test_suite, "remove value valid key", test_remove_one_value_existing_key) == NULL ||
    CU_add_test(my_test_suite, "remove value empty ht", test_remove_value_empty_ht) == NULL ||
    CU_add_test(my_test_suite, "remove value non-existing key", test_remove_value_non_existing_key) == NULL ||
    CU_add_test(my_test_suite, "remove same value twice", test_remove_value_twice) == NULL ||
    CU_add_test(my_test_suite, "remove not affecting other pairs", test_removing_one_value_not_affecting_other) == NULL ||
    CU_add_test(my_test_suite, "remove in buckets", test_remove_one_value_in_bucket_w_mul_values) == NULL ||
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