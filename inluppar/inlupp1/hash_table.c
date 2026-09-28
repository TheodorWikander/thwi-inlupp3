#include <stdlib.h>
#include <stddef.h> 
#include "hash_table.h"

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
    free(ht);
}
