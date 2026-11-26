#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "common.h"


/**
* @file hash_table.h
* @author Nova Edlund
* @date 2026.09.28
* @brief Simple hash table that maps string keys to integer values.
*
* Violating a precondition will crash the program (assert)
*/ 

typedef struct hash_table ioopm_hash_table_t;

/// @brief Create a new hash table
/// @param hash_fn the hash function the ht shold use
/// @param key_eq_fn the eq function the ht should use to compare keys
/// @pre hash_fn and key_eq_fn cannot be null
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @pre key is not NULL
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @param result a pointer to the value mapped to by key
/// @return true if the lookup operation succeded, false otherwise
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @param result pointer to the value mapped to by key (the value to be removed)
/// @return true if key/value mapping was removed succesfully, false outherwise
bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result);

/// @brief looks for key in ht
/// @param ht the ht to look for key in
/// @param key the key to look for
/// @return true if key was found in ht, or false otherwise
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief checks if an ht is empty
/// @param ht the ht to be checked
/// @return true if empty, false if an entry is found in ht
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief For every bucket, count the number of entries in that bucket and sum it all up. 
/// @param ht the hash table to have its entries counted
/// @return the number of entries in ht
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);