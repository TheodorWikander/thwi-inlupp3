#include "hash_table.h"
#include "iterator.h"
#include "common.h"
#include <stdbool.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <string.h>

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

//Change this function if you want to do something BEFORE you run a test suite 
int init_suite(void) {
    return 0;
}

//Change this function if you want to do something AFTER you run a test suite
int clean_suite(void) {
    return 0;
}

void test_iterate_over_empy_ht() {
    //create ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    //create iterator
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

    //check that iterator is at end of ht
    CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

    //destroy iterator and ht
    ioopm_hash_table_iterator_destroy(it);
    ioopm_hash_table_destroy(ht);
}

void test_iterate_over_ht_w_one_entry() {
    //create ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq);;

    elem_t result = int_elem(0);

    //insert key/value pair and check mapping is correct
    ioopm_hash_table_insert(ht, int_elem(123), int_elem(1));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(123), &result));
    CU_ASSERT_EQUAL(1, result.i);

    //create iterator
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
    //check if iterator is at the end of ht (it should not be)
    CU_ASSERT_FALSE(ioopm_hash_table_iterator_at_end(it));

    //check that our current key is the same as our iterators current key
    CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_key(it).i, 123);

    //check that our current value is the same as our iterators current value
    CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it).i, 1);

    //move iterator on step foward
    ioopm_hash_table_iterator_advance(it);

    //check that we are now at the end of ht
    CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

    //destroy iterator and ht
    ioopm_hash_table_iterator_destroy(it);
    ioopm_hash_table_destroy(ht);
}

void test_iterator_several_entries()
{
  elem_t keys[3] = {string_elem("abc"), string_elem("qwe"), string_elem("asd")};
  elem_t values[3] = {int_elem(0), int_elem(1), int_elem(2)};

  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);;
  for (size_t i = 0; i != 3; ++i)
  {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }

  size_t iteration_count = 0;

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    iteration_count++;
    ioopm_hash_table_iterator_advance(it);
  }

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
  CU_ASSERT_EQUAL(iteration_count, 3);
}

void test_iterator_several_entries_once() {

    elem_t keys[3] = {string_elem("abc"), string_elem("qwe"), string_elem("asd")};
    elem_t values[3] = {int_elem(1), int_elem(2), int_elem(3)};

    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    for (size_t i = 0; i != 3; ++i) {
        ioopm_hash_table_insert(ht, keys[i], values[i]);
    }

    size_t iteration_count = 0;
    size_t seen[3] = {0};

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
    
    while (!ioopm_hash_table_iterator_at_end(it)) {

        elem_t current_key = ioopm_hash_table_iterator_current_key(it);

        for (int i = 0; i < 3; ++i) {
            if (strcmp(current_key.s, keys[i].s) == 0) {
                CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it).i, values[i].i);
                ++seen[i];
            }
        }
        ++iteration_count;
        ioopm_hash_table_iterator_advance(it);
    }

    for (int i = 0; i < 3; ++i) {
        CU_ASSERT_EQUAL(seen[i],1);
    }

    ioopm_hash_table_iterator_destroy(it);
    ioopm_hash_table_destroy(ht);
}

//om vi infogar fler nycklar än buckets kommer det bli kollisioner och
//att nycklar delar bucket oavsett hash funktion, Gör detta med hjälp av
//for loop
#define N 50
void test_iterate_over_mul_entries_same_bucket() {

    char keys[N][16];
    //create ht
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

    for (size_t i = 0; i < N; ++i) {
        sprintf(keys[i], "key%ld", i);
        ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem(i));
    }
    size_t seen[N] = {0};
    size_t iteraton_count = 0;

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

    while (!ioopm_hash_table_iterator_at_end(it)) {
        elem_t current_key = ioopm_hash_table_iterator_current_key(it);

        for (size_t i = 0; i < N; ++i) {
            if (strcmp(current_key.s, keys[i]) == 0) {
                CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it).i, i);
                ++seen[i];
            }
        }
        ++iteraton_count;
        ioopm_hash_table_iterator_advance(it);
    }
    CU_ASSERT_EQUAL(iteraton_count, N);

    for (size_t i = 0; i < N; ++i) {
        CU_ASSERT_EQUAL(seen[i], 1);
    }

    ioopm_hash_table_iterator_destroy(it);
    ioopm_hash_table_destroy(ht);
}


int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) 
        return CU_get_error();

    CU_pSuite my_test_suite = CU_add_suite("iterator_tests", init_suite, clean_suite);
    if (my_test_suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
if (CU_add_test(my_test_suite, "iterate over empty ht", test_iterate_over_empy_ht) == NULL ||
    CU_add_test(my_test_suite, "iterate oer array with one entry", test_iterate_over_ht_w_one_entry) == NULL ||
    CU_add_test(my_test_suite, "iterate over ht multiple entries", test_iterator_several_entries) == NULL ||
    CU_add_test(my_test_suite, "iterate over multiple entries once", test_iterator_several_entries_once) == NULL ||
    CU_add_test(my_test_suite, "iterate over ht serveral entries same bucket", test_iterate_over_mul_entries_same_bucket) == NULL ||
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