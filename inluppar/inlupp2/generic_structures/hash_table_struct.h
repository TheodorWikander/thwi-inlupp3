#pragma once

#include "common.h"

#define No_buckets 17

typedef struct entry entry_t;

struct entry {
    elem_t key; //hold the key
    elem_t value; // holds the value
    entry_t *next; // points to next entry (Can be NULL)
};

struct hash_table {
    entry_t buckets[No_buckets];
    ioopm_hash_function *hash_fn;
    ioopm_eq_function *key_eq_fn;
}; 