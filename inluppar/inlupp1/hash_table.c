#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h> 
#include "hash_table.h"
#include <string.h>

typedef struct entry entry_t;

struct entry {
    char *key; //hold the key
    int value; // holds the value
    entry_t *next; // points to next entry (Can be NULL)
};

struct hash_table {
    //DODGE: hard coding number of buckets as 17
    //NOTE: adressning this dodge is optional
    entry_t *buckets[17];
};

ioopm_hash_table_t *ioopm_hash_table_create(void) {
    //allocate space for an ioopmhash_table_t = 17 pointers to entry_t's and by using calloc initializes each value to NULL
    return calloc(1, sizeof(ioopm_hash_table_t));
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    for (int i = 0; i < 17; ++i) {
        entry_t *current = ht->buckets[i];
        while (current != NULL) {
            entry_t *next = current->next;
            free(current);
            current = next;
        }
    }
    free(ht);
}

static size_t string_knr_hash(const char *str) {
    size_t result = 0;
    while (*str != '\0') {
        result = result * 31 + ((unsigned char) *str);
        str++;
    }
    return result;
}

//@precondition key is not NULL
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, char *key, int value) {
    //find bucket
    size_t bucket = string_knr_hash(key) % 17;

    //look for an entry with the key we want
    entry_t *current = ht->buckets[bucket];
    while (current != NULL && strcmp(current->key, key) != 0)
    {
        current = current->next;
    }

    //if the key exists, update the value, otherwise add a 
    //new entry to the end of the list 
    if (current != NULL) {
        current->value = value;
    }
    else {
        //if the bucket is empty, add a new first node
        if (ht->buckets[bucket] == NULL) {
            ht->buckets[bucket] = malloc(sizeof(entry_t));
            ht->buckets[bucket] -> key = key;
            ht->buckets[bucket] -> value = value;
            ht->buckets[bucket] -> next = NULL;
        }
        else {
            //otherwise we append a new node to the list
            entry_t *last = ht->buckets[bucket];
            while (last->next != NULL)
            {
                last = last->next;
            }
            last->next = malloc(sizeof(entry_t));
            last->next->key = key;
            last->next->value = value;
            last->next->next = NULL;
        }
    }
}

//Returns true if the lookup succeded and writes the value stored at key (result) to the memory location result point to
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, char *key, int *result) {
    //find bucket
    size_t bucket = string_knr_hash(key) % 17;

    //look for an entry with the key we want
    entry_t *current = ht->buckets[bucket];
    while (current != NULL && strcmp(current->key, key) != 0)
    {
        current = current->next;
    }
    
    //if the key exists return the value, otherwise indicate that the
    // lookup failed
    if (current != NULL) {
        *result = current->value;
        return true;
    }
    else {
        return false;
    }
}