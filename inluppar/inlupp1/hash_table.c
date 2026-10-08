#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h> 
#include "hash_table.h"
#include "hash_table_struct.h"
#include <string.h>
#include <assert.h>

/// @brief creates a new ht entry
/// @param key the key to map to value in ht
/// @param value the value to be found at key in ht
/// @param next the next entry in ht
/// @return the new entry
static entry_t *entry_create(elem_t key, elem_t value, entry_t *next) {
    entry_t *entry = malloc(sizeof(entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = next;
    return entry;
}
/// @brief destroys an entry
/// @param entry the entry to be destroyed
static void entry_destroy(entry_t *entry) {
    free(entry);
}

/// @brief counts the number of entries in a bucket
/// @param bucket the bucket to count entries in
/// @return the number of entries in the provided bucket 
static size_t bucket_size(entry_t *bucket) {
    size_t i = 0;
    entry_t *current = bucket;
    while (current != NULL) {
        ++i;
        current = current->next;
    } 
    return i; 
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn) {
    assert(hash_fn != NULL && key_eq_fn != NULL);

    ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));

    ht->hash_fn = hash_fn;
    ht->key_eq_fn = key_eq_fn;
    return ht;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    for (size_t i = 0; i < No_buckets; ++i) {
        entry_t *current = ht->buckets[i].next;
        while (current != NULL) {
            entry_t *next = current->next; // save before free
            entry_destroy(current);
            current = next;
        }
    }
    free(ht);
}

//not needed since hash_function is now fprovided in function call
// static size_t string_knr_hash(const char *str) {
//     size_t result = 0;
//     while (*str != '\0') {
//         result = result * 31 + ((unsigned char) *str);
//         str++;
//     }
//     return result;
// }

static entry_t *find_previous_entry(ioopm_hash_table_t *ht, elem_t key) {
    //find bucket
    size_t bucket = ht->hash_fn(key) % No_buckets;

    //create a pointer to the first element in each bucket (sentinel node)
    entry_t *previous = &ht->buckets[bucket];

    //look for the element where the next element is either NULL or has the .key == key
    while (previous->next != NULL && !ht->key_eq_fn(previous->next->key, key))
        { 
        previous = previous->next;
        } 
    return previous;
}

//@precondition key is not NULL
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) {
    
    //find the previous entry, or the last entry if the key does not exist
    entry_t *previous = find_previous_entry(ht, key);

    //if the key exists, update the value, otherwise create a new entry
    if (previous->next != NULL) {
        previous->next->value = value;
    } else {
        previous->next = entry_create(key, value, NULL);
    }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result) {
    
    entry_t *previous = find_previous_entry(ht, key);

    if (previous->next == NULL) {
        return false;
    }
    *result = previous->next->value;
    return true;
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result) {
   
    entry_t *previous = find_previous_entry(ht, key);

    if (previous->next == NULL) {
        return false;
    } else {
        entry_t *to_remove = previous->next;
        *result = to_remove->value;
        previous->next =to_remove->next;
        entry_destroy(to_remove);
        return true;
    }
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key) {
    //will find eother the elemet before the key or the elelemt before NULL
    entry_t *prevoius = find_previous_entry(ht, key);
    //if next element is null, key was not in ht 
     if (prevoius->next == NULL) {
        return false;
     } else {
        return true;
     }
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht) {
    // for every bucket, count the number of entries in the bucket and add to size 
    size_t size = 0;
    for (size_t i = 0; i < No_buckets; ++i) {
        size += bucket_size(ht->buckets[i].next);
    }
    return size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht) {
    //if node after sentinel node in any bucket is not NULL, ht is not empty
    for (size_t i = 0; i < No_buckets; ++i) {
        if (ht->buckets[i].next != NULL) {
            return false;
        }
    }
    return true;
}

    