#include "hash_table.h"
#include "common.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

static bool string_eq(elem_t a, elem_t b) {
    return strcmp(a.s, b.s) == 0;
}

static size_t string_hash(elem_t key) {
    size_t result = 0;
    for (const char *s = key.s; *s != '\0'; ++s) {
        result = result * 31 + (unsigned char) *s;
    }
    return result;
}

static bool int_eq(elem_t a, elem_t b) {
    return a.i == b.i;
}

static size_t int_hash(elem_t key) {
    return (size_t) key.i;
}

static size_t forced_collision(elem_t key) {
    (void) key;
    return 0;
}

//Change this function if you want to do something BEFORE you run a test suite 
int init_suite(void) {
    return 0;
}

//Change this function if you want to do something AFTER you run a test suite
int clean_suite(void) {
    return 0;
}

void test_create_destroy() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
    CU_ASSERT_PTR_NOT_NULL(ht);
    ioopm_hash_table_destroy(ht);
}

void test_insert_once() {
    //create new ht 
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t key = string_elem("abc");

    //check that key is not in ht 
    elem_t result = int_elem(0); 
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, 0);

    //insert key-value pair and check that the mapping exists
    ioopm_hash_table_insert(ht, key, int_elem(123));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, 123);

    //destroy hash table
    ioopm_hash_table_destroy(ht);
}

void test_insert_int_key_once(void) {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq);

    elem_t key = int_elem(1);

    elem_t result = int_elem(0);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, 0);

    ioopm_hash_table_insert(ht, key, int_elem(123));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, 123);

    ioopm_hash_table_destroy(ht);
}

void test_insert_old_key_new_value() {
    //create new ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t key = string_elem("abc");
    elem_t result = int_elem(0);
    //insert key value pair
    ioopm_hash_table_insert(ht, key, int_elem(123));

    //use same key and new value
    ioopm_hash_table_insert(ht, key, string_elem("abc"));

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.s, "abc");

    //destroy hash table
    ioopm_hash_table_destroy(ht);
}

void test_lookup_non_existing_key() {
    //create new ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);;

    elem_t key = string_elem("abc");

    //check that key is not in ht
    elem_t result = int_elem(0);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, 0);

     //insert key-value pair
    ioopm_hash_table_insert(ht, key, int_elem(123));

    //Try to lookup a key that is not in ht
    elem_t fakekey = string_elem("xyz");
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, fakekey, &result));
    CU_ASSERT_EQUAL(result.i, 0);

    //destroy hash table
    ioopm_hash_table_destroy(ht);
}

void test_empty_string_as_key() {
   ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t key = string_elem("");

    //check that key is not already in ht
    elem_t result = int_elem(0);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, 0);

    //insert key-value pair and check that the mapping exists
    ioopm_hash_table_insert(ht, key, int_elem(123));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, 123);

    //destroy hash table
    ioopm_hash_table_destroy(ht);
}

void test_updating_value_does_not_affect_other_keys() {
    //create ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t key = string_elem("abc");
    elem_t result = int_elem(0);

    //insert key-value pair and check that the mapping exists
    ioopm_hash_table_insert(ht, key, int_elem(123));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, 123);

    //insert new key-value pair
    elem_t key2 = string_elem("xyz");
    elem_t result2 = int_elem(0);

    ioopm_hash_table_insert(ht, key2, int_elem(987));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
    CU_ASSERT_EQUAL(result2.i, 987);

    //change value at key 1 and check that value is updated
    ioopm_hash_table_insert(ht, key, int_elem(321));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, 321);

    //check that value at key2 remains the same
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
    CU_ASSERT_EQUAL(result2.i, 987);


    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_insert_colliding_keys() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(forced_collision, string_eq);
    elem_t result = int_elem(0);

    ioopm_hash_table_insert(ht, string_elem("a"), int_elem(1));
    ioopm_hash_table_insert(ht, string_elem("r"), string_elem("a")); 
    ioopm_hash_table_insert(ht, string_elem("P"), bool_elem(true)); 

    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 3);

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("r"), &result));
    CU_ASSERT_STRING_EQUAL(result.s, "a");
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("P"), &result));
    CU_ASSERT_EQUAL(result.b, true);

    //overwrite value in the middle of the chain
    ioopm_hash_table_insert(ht, string_elem("r"), int_elem(20));

    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 3);

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("r"), &result));
    CU_ASSERT_EQUAL(result.i, 20);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("P"), &result));
    CU_ASSERT_EQUAL(result.b, true);

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_remove_one_value_existing_key() {
    //create new ht 
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t key = string_elem("abc");
    elem_t result = int_elem(0); 
    elem_t removed_value = int_elem(0);

    //insert key-value pair and check that the mapping exists
    ioopm_hash_table_insert(ht, key, int_elem(123));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, 123);

    //remove value stored at key and save the removed value in the varable removed_value
    //Check that the value removed is the value we intended. 
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key, &removed_value));
    CU_ASSERT_EQUAL(removed_value.i, 123);

    //COntroll that value is no longer in ht
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
    
    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_remove_value_empty_ht() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t key = string_elem("abc");
    elem_t removed_value = int_elem(0);

    CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, key, &removed_value));
    CU_ASSERT_EQUAL(removed_value.i, 0);

    ioopm_hash_table_destroy(ht);
}

void test_remove_value_non_existing_key() {
     //create new ht 
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t key = string_elem("abc");
    elem_t value = int_elem(123);
    elem_t key2 = string_elem("xyz");
    elem_t value2 = int_elem(987);
    elem_t key3 = string_elem("i");
    elem_t result = int_elem(0); 
    elem_t result2 = int_elem(0); 
    elem_t removed_value = int_elem(0);

    //insert key-value pairs and check that the mappings exists
    ioopm_hash_table_insert(ht, key, value);
    ioopm_hash_table_insert(ht, key2, value2);

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, value.i);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
    CU_ASSERT_EQUAL(result2.i, value2.i);

    //try to remove a value at a non existing key. 
    CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, key3, &removed_value));
    CU_ASSERT_EQUAL(removed_value.i, 0);

    //check that the other mappins are intact after failed remove
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, value.i);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
    CU_ASSERT_EQUAL(result2.i, value2.i);

    //remove ht
    ioopm_hash_table_destroy(ht);
}

void test_remove_value_twice() {
     //create new ht 
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t key = string_elem("abc");
    elem_t value = int_elem(123);
    elem_t result = int_elem(0);
    elem_t removed_value = int_elem(0);

    //insert pair and check that mapping exists and is correct
    ioopm_hash_table_insert(ht, key, value);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(value.i, result.i);

    //remove value and check that the value removed was the right one
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key, &removed_value));
    CU_ASSERT_EQUAL(removed_value.i, value.i);

    //Double check value is not in ht
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

    //try to remove the same value again and check that it does not work
    CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, key, &removed_value));

    //controll that removed value has not changed when remove failed
    CU_ASSERT_EQUAL(removed_value.i, 123);

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_removing_one_value_not_affecting_other() {
    //create new ht 
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t key = string_elem("abc");
    elem_t value = int_elem(123);
    elem_t key2 = string_elem("xyz");
    elem_t value2 = int_elem(987);
    elem_t result = int_elem(0); 
    elem_t result2 = int_elem(0); 
    elem_t removed_value = int_elem(0);

    //insert key-value pairs and check that the mappings exists
    ioopm_hash_table_insert(ht, key, value);
    ioopm_hash_table_insert(ht, key2, value2);

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, value.i);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
    CU_ASSERT_EQUAL(result2.i, value2.i);

    //remove one value and check that the correct value was removed
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key, &removed_value));
    CU_ASSERT_EQUAL(removed_value.i, value.i);

    //check that the removed value is actualy removed
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

    //check that the other pair remain intact still
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));
    CU_ASSERT_EQUAL(result2.i, value2.i);

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_remove_one_value_in_bucket_w_mul_values() {
    //create new ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
    
    elem_t result = int_elem(0);
    elem_t removed_value = int_elem(0);
    
    //insert 3 separate pairs in the same bucket  (hashfunc % 17 puts them all in bucket 12)
    ioopm_hash_table_insert(ht, string_elem("a"), int_elem(1));
    ioopm_hash_table_insert(ht, string_elem("r"), string_elem("a"));
    ioopm_hash_table_insert(ht, string_elem("P"), bool_elem(true)); 

    //Check each pair was inserted correctly
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("r"), &result));
    CU_ASSERT_STRING_EQUAL(result.s, "a");
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("P"), &result));
    CU_ASSERT_EQUAL(result.b, true);

    //remove value in the middle
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("r"), &removed_value));

    //check that the right value was removed and is not still in ht
    CU_ASSERT_EQUAL(removed_value.s, "a");
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem("r"), &result));

    //check that the other pairs are still intact
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("P"), &result));
    CU_ASSERT_EQUAL(result.b, true);

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

// Fler test som kan skapas:
// Ta bort första och sista noden i en kedja
// Infoga ta bort och infoga igen med samma nyckel
// Många nycklar

//Create a hash table and check if it has some key k (it shouldn’t).
void test_check_if_empty_ht_has_key() {

ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

elem_t key = string_elem("abc");

CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, key));

ioopm_hash_table_destroy(ht);
}

//Create a hash table and insert some key k. Check if it has the 
//key k (it should). Also check if it has some other key k2 
//(it shouldn’t).
void test_check_for_existing_and_non_existing_key() {
    //create ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    //initialize parameters
    elem_t key = string_elem("abc");
    elem_t value = int_elem(123);
    elem_t result = int_elem(0);

    //insert key-value pair and check that the mapping exists
    ioopm_hash_table_insert(ht, key, value);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
    CU_ASSERT_EQUAL(result.i, value.i);

    //check if key "abc" exist (it should)
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key));

    //check if key "xyz" exist (it should not)
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("xyz")));

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

//Create a hash table and insert keys k1, k2 and k3. Check if all 
//three keys are present (they should be). Also check if it has 
//some other key k4 (it shouldn’t).
void test_check_multiple_existing_and_one_non_existing_key() {
    //create new ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t result = int_elem(0);

    //insert 3 pairs and check that mapping exist and is correct
    ioopm_hash_table_insert(ht, string_elem("a"), int_elem(1));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);

    ioopm_hash_table_insert(ht, string_elem("b"), string_elem("a"));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("b"), &result));
    CU_ASSERT_STRING_EQUAL(result.s, "a");

    ioopm_hash_table_insert(ht, string_elem("c"), bool_elem(false));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("c"), &result));
    CU_ASSERT_EQUAL(result.b, false);

    //check that all three keys are present (should be)
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("a")));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("b")));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("c")));

    //check if another key is present (should not be)
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("d")));

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

//Create a hash table and insert key k, and immediately 
//remove it. Check if has key k (it shouldn’t).
void test_check_for_removed_key() {
    //create new ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t result = int_elem(0);

    //insert key/value pair and check mapping exist and is correct
    ioopm_hash_table_insert(ht, string_elem("a"), int_elem(1));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);

    //remove value at key and check that correct value was removed
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);

    //Check that value is no longer in ht
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));

    //Check that key is not longer in ht
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("a")));

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

//Create a hash table and insert keys k1, k2 and k3. 
//Remove key k2. Check if it has keys k1 and k3 (it should). 
//Check if it has key k2 (it shouldn’t).
void test_check_for_key_removed_in_the_middle() {
    //create new ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t result = int_elem(0);

    //insert 3 pairs and check that mapping exist and is correct
    ioopm_hash_table_insert(ht, string_elem("a"), int_elem(1));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);

    ioopm_hash_table_insert(ht, string_elem("b"), string_elem("abc"));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("b"), &result));
    CU_ASSERT_STRING_EQUAL(result.s, "abc");

    ioopm_hash_table_insert(ht, string_elem("c"), bool_elem(true));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("c"), &result));
    CU_ASSERT_EQUAL(result.b, true);

    //remove key 2, check taht the right value was removed
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("b"), &result));
    CU_ASSERT_STRING_EQUAL(result.s, "abc");

    //Check that key 1 and 3 still exists (they should)
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("a")));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("c")));

    //check if key 2 still exist (it should not)
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("b")));

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

//can write another test like the one above but testing the same
//scenario but with the keys being in the same bucket

void test_check_if_empty_ht_is_empty() {
    //create ht 
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    //check that ht is empty
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_check_if_non_empty_ht_is_empty() {
    //create ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t result = int_elem(0);

    //insert pair check mapping
    ioopm_hash_table_insert(ht, string_elem("a"), int_elem(1));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);

    //check if ht is empty (should not be)
    CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

    //destroy ht
    ioopm_hash_table_destroy(ht);

}

void test_remove_entry_and_check_if_empty() {
    //create ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t result = int_elem(0);

    //insert 3 pairs
    ioopm_hash_table_insert(ht, string_elem("a"), int_elem(1));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);

    ioopm_hash_table_insert(ht, string_elem("b"), int_elem(2));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("b"), &result));
    CU_ASSERT_EQUAL(result.i, 2);

    ioopm_hash_table_insert(ht, string_elem("c"), int_elem(3));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("c"), &result));
    CU_ASSERT_EQUAL(result.i, 3);

    //remove one entry, check that right value was removed
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("b"), &result));
    CU_ASSERT_EQUAL(result.i, 2);

    //Check if ht is empty (should not be)
    CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

    //destroy ht
    ioopm_hash_table_destroy(ht);
}

void test_remove_all_entries_and_check_if_empty() {
    //create ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    elem_t result = int_elem(0);

    //insert 2 pairs
    ioopm_hash_table_insert(ht, string_elem("a"), int_elem(1));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);

    ioopm_hash_table_insert(ht, string_elem("b"), string_elem("abc"));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("b"), &result));
    CU_ASSERT_EQUAL(result.s, "abc");
    
    //remove 2 pairs
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("a"), &result));
    CU_ASSERT_EQUAL(result.i, 1);
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("b"), &result));
    CU_ASSERT_EQUAL(result.s, "abc");

    //check if ht is empty (should be) 
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

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
    CU_add_test(my_test_suite, "insert integer key", test_insert_int_key_once) == NULL ||
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
    CU_add_test(my_test_suite, "look for non-existing key in empty ht", test_check_if_empty_ht_has_key) == NULL ||
    CU_add_test(my_test_suite, "look for one existing and one non-existing key in ht", test_check_for_existing_and_non_existing_key) == NULL ||
    CU_add_test(my_test_suite, "look for multiple existing kes in ht and one non-existing", test_check_multiple_existing_and_one_non_existing_key) == NULL ||
    CU_add_test(my_test_suite, "look for removed key", test_check_for_removed_key) == NULL ||
    CU_add_test(my_test_suite, "look for multiple keys and one key removed in the middle", test_check_for_key_removed_in_the_middle) == NULL ||
    CU_add_test(my_test_suite, "check if empty ht is empty", test_check_if_empty_ht_is_empty) == NULL ||
    CU_add_test(my_test_suite, "check if non empty ht is empty", test_check_if_non_empty_ht_is_empty) == NULL ||
    CU_add_test(my_test_suite, "remove on of many entries and check if empty", test_remove_entry_and_check_if_empty) == NULL ||
    CU_add_test(my_test_suite, "remove all entires and check if empty", test_remove_all_entries_and_check_if_empty) == NULL ||
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