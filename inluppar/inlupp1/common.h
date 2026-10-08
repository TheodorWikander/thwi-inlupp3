#pragma once

#include <stdbool.h>
#include <stddef.h>

/** 
 * @file common.h
 * @brief Types shared by the linked list and the hash table 
*/


///helpers for constructing an elem_t and how to read it back
#define int_elem(x) ((elem_t) { .i = (x)})
#define bool_elem(x) ((elem_t) { .b = (x)})
#define ptr_elem(x) ((elem_t) { .p = (x)})
#define string_elem(x) ((elem_t) { .s = (x)})
#define unsigned_elem(x) ((elem_t) { .u = (x)})

typedef union elem elem_t;

/// @brief A value of one or several types. User must keep track
/// of which field is being used and should be read from
union elem {
    int i;
    unsigned int u;
    bool b;
    float f;
    void *p;
    char *s;
};

/// @brief Equality function for keys
/// @return true iff a and b are equal
typedef bool ioopm_eq_function(elem_t a, elem_t b);

/// @brief Hash function for keys
/// @return A hash for a key. Keys who are equal according to 
/// ioopm_eq_function and used together with this function
/// must have equal hashes
typedef size_t ioopm_hash_function(elem_t key);