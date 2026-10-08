#include "hash_table.h"
#include "iterator.h"
#include "hash_table_struct.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

struct hash_table_iterator {
    ioopm_hash_table_t *ht;
    size_t current_bucket;
    entry_t *current_entry;
}; 

/// @brief advance the iterator one step foward in the ht
/// @param it the iterator to be advanced
static void advance_iterator_state(ioopm_hash_table_iterator_t *it) {
    //advnce to the next entry in the bucket
    it->current_entry = it->current_entry->next;

    //if it was null advance to the next bucket
    if (it->current_entry == NULL) {
        it->current_bucket += 1;
        
        //if the next bucket existed, update the current entry
        if (it->current_bucket != No_buckets) {
            it->current_entry = &it->ht->buckets[it->current_bucket];
        }
    }
}

/// @brief if it is pointing to a sentinel node and is not at the end of ht, skip to the next node. 
/// @param it the iterator to be advanced 
static void skip_sentinel_nodes(ioopm_hash_table_iterator_t *it) {
    while (it->current_bucket != No_buckets &&
    it->current_entry == &it->ht->buckets[it->current_bucket])
    {
        advance_iterator_state(it); 
    } 
}

ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht) {
    ioopm_hash_table_iterator_t *it = calloc(1, sizeof(ioopm_hash_table_iterator_t));
    it->ht = ht;
    it->current_bucket = 0;
    it->current_entry = &ht->buckets[0];
    skip_sentinel_nodes(it);
    return it;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it) {
    free(it);
}

bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it) {
    return it->current_bucket == No_buckets;
}

void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it) {
    if (it->current_entry == NULL) {
        return; 
    }
        advance_iterator_state(it);
        skip_sentinel_nodes(it);
}

elem_t ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it) {
    //craches program if current_entry == NULL
    assert(it->current_entry != NULL);

    return it->current_entry->key;
}

elem_t ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it) {
    //crashes program if curretn key == NULL
    assert(it->current_entry != NULL);

    return it->current_entry->value;
}
