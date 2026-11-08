#include "store.h"
#include <stdlib.h>
#include <string.h>


struct location {
    char *shelf;
    int quantity;
};

struct merch {
    char *name;
    char *description;
    int price;
    ioopm_list_t *locations;
};

struct carts{
    int id;
    ioopm_hash_table_t *items; 
};

struct store {
    ioopm_hash_table_t *merch_db; //name -> mech_t
    ioopm_hash_table_t *shelf_db; //shelf-> name of merch/count
    ioopm_hash_table_t *carts_db;
    int next_cart_id;
};

store_t *ioopm_store_create(void) {
    store_t *store = calloc(1, sizeof(store_t));

    store->merch_db = ioopm_hash_table_create(string_hash, string_eq);
    store->shelf_db = ioopm_hash_table_create(string_hash, string_eq);
    store->carts_db = ioopm_hash_table_create(int_hash, int_eq);
    store->next_cart_id = 1;
    return store;
}

/// @brief 
/// @param merch 
static void merch_destroy(merch_t *merch) {
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->locations);
    while (!ioopm_list_iterator_at_end(it)) {
        location_t *loc = ioopm_list_iterator_current(it).p;
        free(loc->shelf);
        free(loc);
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);

    ioopm_list_destroy(merch->locations);
    free(merch->name);
    free(merch->description);
    free(merch);
}

void ioopm_store_destroy(store_t *store) {
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(store->merch_db);
    while(!ioopm_hash_table_iterator_at_end(it)) {
        merch_t *merch = ioopm_hash_table_iterator_current_value(it).p;
        merch_destroy(merch);
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);

    ioopm_hash_table_destroy(store->merch_db);
    ioopm_hash_table_destroy(store->shelf_db);
    ioopm_hash_table_destroy(store->carts_db);
    free(store);

}

char *ioopm_merch_name(merch_t *merch) {
    return merch->name;
}

char *ioopm_merch_description(merch_t *merch) {
    return merch->description;
}

size_t ioopm_merch_price(merch_t *merch) {
    return merch->price;
}

/// @brief 
/// @param name 
/// @param desc 
/// @param price 
/// @return 
static merch_t *create_merch(char *name, char *desc, int price) {
    merch_t *merch = calloc(1, sizeof(merch_t));

    merch->name = strdup(name);
    merch->description = strdup(desc);
    merch->price = price;
    merch->locations = ioopm_list_create();

    return merch;
}

bool ioopm_store_has_merch(store_t *store, char *name) {
    elem_t key = string_elem(name);
    if (ioopm_hash_table_has_key(store->merch_db, key)) return true;
    return false;
}

bool ioopm_store_add_merch(store_t *store, char *name, char *desc, int price) {
    if (ioopm_store_has_merch(store, name)) return false;

    merch_t *merch = create_merch(name, desc, price);

    ioopm_hash_table_insert(store->merch_db, string_elem(merch->name), ptr_elem(merch));
    return true;
}

size_t ioopm_store_merch_count(store_t *store) {
    return ioopm_hash_table_size(store->merch_db);
}

/// @brief get the merch item with the name "name" if it exists
/// @param store the store we look for the merch in
/// @param name the name of the merch we are looking for
/// @return merch item with name as its name of null if merch is not in store
merch_t *ioopm_store_get_merch(store_t *store, char *name) {
    elem_t result;
    if (ioopm_hash_table_lookup(store->merch_db, string_elem(name), &result)) {
        return result.p;
    }
    return NULL;
}

static void unregister_shelves(store_t *store, merch_t *merch) {
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->locations);
    while (!ioopm_list_iterator_at_end(it)) {
        location_t *loc = ioopm_list_iterator_current(it).p;
        elem_t ignored;
        ioopm_hash_table_remove(store->shelf_db, string_elem(loc->shelf), &ignored);
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);
}

/// @brief removes merch item with name "name"
/// @param store the store to look for merch in
/// @param name the name of the merch we are looking for
/// @return true if merch is succesfully removed from store, false otherwise
bool ioopm_store_remove_merch(store_t *store, char *name) {
    elem_t result;
    if (ioopm_hash_table_remove(store->merch_db, string_elem(name), &result)) {
       merch_t *merch = result.p;
       unregister_shelves(store, merch);
       merch_destroy(merch);
       return true;
    }
    return false;
}

/// @brief editds merch with name "old_name" and updated it with new_name, new_desc and new_price
/// @param store the store merch is in
/// @param old_name the old name to identify the merch to be edited
/// @param new_name the new name to give the merch
/// @param new_desc the new description of the merch
/// @param new_price the new peice of the merch
/// @return true if edit was succesfull, false outerwise
bool ioopm_store_edit_merch(store_t *store, char *old_name, char *new_name, char *new_desc, int new_price) {
    if (!ioopm_store_has_merch(store, old_name)) return false;

    bool name_changes = strcmp(old_name, new_name) != 0;
    if (name_changes && ioopm_store_has_merch(store, new_name)) return false;

    elem_t result;
    ioopm_hash_table_remove(store->merch_db, string_elem(old_name), &result);
    merch_t *merch = result.p;

    free(merch->name);
    free(merch->description);
    merch->name = strdup(new_name);
    merch->description = strdup(new_desc);
    merch->price = new_price;

    ioopm_hash_table_insert(store->merch_db, string_elem(merch->name), ptr_elem(merch));

    //TODO: uppdatera varunamnet i shelf_db när replenish finns

    return true;
}

static bool valid_shelf(char *shelf) {
    return strlen(shelf) == 3
        && shelf[0] >= 'A' && shelf[0] <= 'Z'
        && isdigit((unsigned char) shelf[1])
        && isdigit((unsigned char) shelf[2]);
}

static location_t *find_location(merch_t *merch, char *shelf) {
    location_t *found = NULL;
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->locations);
    while (!ioopm_list_iterator_at_end(it)) {
        location_t *loc = ioopm_list_iterator_current(it).p;
        if (strcmp(loc->shelf, shelf) == 0) found = loc;
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);
    return found;
}

bool ioopm_store_replenish(store_t *store, char *shelf, char *name, int amount) {
    if (amount < 1 || !valid_shelf(shelf)) return false;
    merch_t *merch = ioopm_store_get_merch(store, name);
    if (merch == NULL) return false;

    elem_t owner;
    if (ioopm_hash_table_lookup(store->shelf_db, string_elem(shelf), &owner)) {
        if (owner.p != merch) return false;
        find_location(merch, shelf)->quantity += amount;
        return true;
    }

    location_t *loc = calloc(1, sizeof(location_t));
    loc->shelf = strdup(shelf);
    loc->quantity = amount;
    ioopm_list_append(merch->locations, ptr_elem(loc));
    ioopm_hash_table_insert(store->shelf_db, string_elem(loc->shelf), ptr_elem(merch));
    return true;
}


int ioopm_store_merch_stock(store_t *store, char *name) {
  merch_t *merch = ioopm_store_get_merch(store, name);
  if (merch == NULL) return 0;

  int total = 0;
  ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->locations);
  while (!ioopm_list_iterator_at_end(it)) {
    location_t *loc = ioopm_list_iterator_current(it).p;
    total += loc->quantity;
    ioopm_list_iterator_advance(it);
  }
  ioopm_list_iterator_destroy(it);
  return total;
}