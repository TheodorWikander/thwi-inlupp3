#include "store.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


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
    ioopm_hash_table_t *merch_db; //name -> merch_t
    ioopm_hash_table_t *shelf_db; //shelf name -> merch_t (the merch that holds the shelf)
    ioopm_hash_table_t *carts_db; //cart id -> carts_t
    int next_cart_id;
};

static void remove_from_all_carts(store_t *store, char *name);

store_t *ioopm_store_create(void) {
    store_t *store = calloc(1, sizeof(store_t));

    store->merch_db = ioopm_hash_table_create(string_hash, string_eq);
    store->shelf_db = ioopm_hash_table_create(string_hash, string_eq);
    store->carts_db = ioopm_hash_table_create(int_hash, int_eq);
    store->next_cart_id = 1;
    return store;
}

/// @brief Frees a merch, including all of its locations, and its struct. 
///        Does not edit the shelf_db or carts
/// @param merch the merch to free
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

/// @brief frees a cart, including the copied merch names used as keys, the items table and the struct. 
///        Keys must be freed before table is destroyed
/// @param cart the cart to free
static void cart_destroy(carts_t *cart) {
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->items);
    while (!ioopm_hash_table_iterator_at_end(it)) {
        free(ioopm_hash_table_iterator_current_key(it).s);
        ioopm_hash_table_iterator_advance(it);
        }
    ioopm_hash_table_iterator_destroy(it);
    ioopm_hash_table_destroy(cart->items);
    free(cart);
}

void ioopm_store_destroy(store_t *store) {
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(store->merch_db);
    while(!ioopm_hash_table_iterator_at_end(it)) {
        merch_t *merch = ioopm_hash_table_iterator_current_value(it).p;
        merch_destroy(merch);
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);

    ioopm_hash_table_iterator_t *cit = ioopm_hash_table_iterator_create(store->carts_db);
    while (!ioopm_hash_table_iterator_at_end(cit)) {
        cart_destroy(ioopm_hash_table_iterator_current_value(cit).p);
        ioopm_hash_table_iterator_advance(cit);
    }

    ioopm_hash_table_iterator_destroy(cit);

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

int ioopm_merch_price(merch_t *merch) {
    return merch->price;
}

/// @brief Creates a new merch item with its own copy of name and description and an empty 
///        locations list (stock 0)
/// @return the new merch, owned by the caller until inserted into the merch_db
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
    if (price < 0 || ioopm_store_has_merch(store, name)) return false;

    merch_t *merch = create_merch(name, desc, price);

    ioopm_hash_table_insert(store->merch_db, string_elem(merch->name), ptr_elem(merch));
    return true;
}

size_t ioopm_store_merch_count(store_t *store) {
    return ioopm_hash_table_size(store->merch_db);
}

merch_t *ioopm_store_get_merch(store_t *store, char *name) {
    elem_t result;
    if (ioopm_hash_table_lookup(store->merch_db, string_elem(name), &result)) {
        return result.p;
    }
    return NULL;
}

/// @brief removes all merch (one kind) from shelf_db so shelves become free
///        must(!) be called before(!) merch_destroy since the keys in shelf_db are the 
///        strings merch_destroy frees
/// @param merch the merch whose shelves are to be unregistered
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

bool ioopm_store_remove_merch(store_t *store, char *name) {
    if (!ioopm_store_has_merch(store, name)) return false;

    remove_from_all_carts(store, name);

    elem_t result;
    ioopm_hash_table_remove(store->merch_db, string_elem(name), &result);
    merch_t *merch = result.p;
    unregister_shelves(store, merch);
    merch_destroy(merch);
    return true;
}

bool ioopm_store_edit_merch(store_t *store, char *old_name, char *new_name, char *new_desc, int new_price) {
    if (new_price < 0 || !ioopm_store_has_merch(store, old_name)) return false;

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

    if (name_changes) {
        ioopm_hash_table_iterator_t *cit = ioopm_hash_table_iterator_create(store->carts_db);
        while (!ioopm_hash_table_iterator_at_end(cit)) {
            carts_t *cart = ioopm_hash_table_iterator_current_value(cit).p;
            elem_t amount;

            if (ioopm_hash_table_lookup(cart->items, string_elem(old_name), &amount)) {
                char *stored_key = NULL;
                ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->items);
                while (!ioopm_hash_table_iterator_at_end(it)) {
                    if (string_eq(string_elem(old_name), ioopm_hash_table_iterator_current_key(it))) {
                        stored_key = ioopm_hash_table_iterator_current_key(it).s;
                        break;
                    }
                    ioopm_hash_table_iterator_advance(it);
                }
                ioopm_hash_table_iterator_destroy(it);

                elem_t removed;
                ioopm_hash_table_remove(cart->items, string_elem(old_name), &removed);
                free(stored_key);

                ioopm_hash_table_insert(cart->items, string_elem(strdup(new_name)), amount);
            }
            ioopm_hash_table_iterator_advance(cit);
        }
        ioopm_hash_table_iterator_destroy(cit);
    }
    return true;
}

/// @brief Checks if the shelf format is valid. (One capital letter A-Z followed by 2 digits)
/// @return true if the shelf string has a valid format
static bool valid_shelf(char *shelf) {
    return strlen(shelf) == 3
        && shelf[0] >= 'A' && shelf[0] <= 'Z'
        && isdigit((unsigned char) shelf[1])
        && isdigit((unsigned char) shelf[2]);
}

/// @brief Finds the location of a merch on a given shelf
/// @return the location or NULL if the merch is not stored on the shelf
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

/// @brief Comparison function for qsort on an array of location_t *
///        qsort passes pointers TO the elements (location_t **)
/// @return 0 if the locations are equal (the same location), negative if a sorts before b, positive if a sorts after b
static int compare_locations(const void *a, const void *b) {
    location_t *la = *(location_t **) a;
    location_t *lb = *(location_t **) b;
    return strcmp(la->shelf, lb->shelf);
}

/// @brief Builds an array of a merchs sorted locations by shelf names.
///        The array is allocated and must then be freed by the caller 
/// @return the array or NULL if the merch does not exist or has no locations
static location_t **sorted_locations(store_t *store, char *name, int *count) {
    *count = 0;
    merch_t *merch = ioopm_store_get_merch(store, name);
    if (merch == NULL) return NULL;

    size_t n = ioopm_list_size(merch->locations);
    if (n == 0) return NULL;

    location_t **array = calloc(n, sizeof(location_t *));
    for (size_t i = 0; i < n; ++i) {
        array[i] = ioopm_list_get(merch->locations, i).p;
    }
    qsort(array, n, sizeof(location_t *), compare_locations);

    *count = (int) n;
    return array;
}

int ioopm_store_shelf_count(store_t *store, char *name) {    
    int count;
    location_t **array = sorted_locations(store, name, &count);
    free(array);
    return count;
}

char *ioopm_store_shelf_name(store_t *store, char *name, int index) {
    int count;
    location_t **array = sorted_locations(store, name, &count);
    assert(index >= 0 && index < count);
    char *shelf = array[index]->shelf;
    free(array);
    return shelf;
}

int ioopm_store_shelf_quantity(store_t *store, char *name, int index) {
    int count;
    location_t **array = sorted_locations(store, name, &count);
    assert(index >= 0 && index < count);
    int quantity = array[index]->quantity;
    free(array);
    return quantity;
}

/// @brief compares two strings
/// @return 0 if the two stings are equal
static int compare_names(const void *a, const void *b) {
    char *ma = *(char **) a;
    char *mb = *(char **) b;
    return strcmp(ma, mb);
}

char *ioopm_store_merch_name_at(store_t *store, int index) {
    int count = (int) ioopm_store_merch_count(store);
    assert(index >= 0 && index < count);

    char **names = calloc(count, sizeof(char *));
    int i = 0;
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(store->merch_db);

    while (!ioopm_hash_table_iterator_at_end(it)) {
        names[i++] = ioopm_hash_table_iterator_current_key(it).s;
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);

    qsort(names, count, sizeof(char *), compare_names);
    char *result = names[index];
    free(names);
    return result;
}

// ---------  C A R T S  ---------

int ioopm_store_cart_create(store_t *store) {
    carts_t *cart = calloc(1, sizeof(carts_t));
    cart->id = store->next_cart_id++;
    cart->items = ioopm_hash_table_create(string_hash, string_eq);
    ioopm_hash_table_insert(store->carts_db, int_elem(cart->id), ptr_elem(cart));
    return cart->id;
}

bool ioopm_store_cart_remove(store_t *store, int cart_id) {
    elem_t result;
    if (ioopm_hash_table_remove(store->carts_db, int_elem(cart_id), &result)) {
        cart_destroy(result.p);
        return true;
    }
    return false;
}

bool ioopm_store_has_cart(store_t *store, int cart_id) {
    return ioopm_hash_table_has_key(store->carts_db, int_elem(cart_id));
}

/// @return the cart with cart_id in store or NULL if cart does not exist
static carts_t *find_cart(store_t *store, int cart_id) {
    elem_t result;
    if (ioopm_hash_table_lookup(store->carts_db, int_elem(cart_id), &result)) {
        return result.p;
    }
    return NULL;
}

/// @brief calcualtes the amount of items (name) resevred across different carts in store
/// @return the amount of a specific item reseved across a store
static int reserved_amount(store_t *store, char *name) {
    int total = 0;
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(store->carts_db);
    while (!ioopm_hash_table_iterator_at_end(it)) {
        carts_t *cart = ioopm_hash_table_iterator_current_value(it).p;
        elem_t amount;
        if (ioopm_hash_table_lookup(cart->items, string_elem(name), &amount)) {
            total += amount.i;
        }
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);
    return total;
}

bool ioopm_store_cart_add(store_t *store, int cart_id, char *name, int amount) {
    carts_t *cart = find_cart(store, cart_id);
    if (amount < 1 || cart == NULL || !ioopm_store_has_merch(store, name)) return false;
    if (amount > ioopm_store_merch_stock(store, name) - reserved_amount(store, name)) return false;

    elem_t current; 
    if (ioopm_hash_table_lookup(cart->items, string_elem(name), &current)) {
        ioopm_hash_table_insert(cart->items, string_elem(name), int_elem(current.i + amount));
    } else {
        ioopm_hash_table_insert(cart->items, string_elem(strdup(name)), int_elem(amount));
    }
    return true;
}

int ioopm_store_cart_item_count(store_t *store, int cart_id, char *name) {
    carts_t *cart = find_cart(store, cart_id);
    if (cart == NULL) return 0;

    elem_t amount;
    if (ioopm_hash_table_lookup(cart->items, string_elem(name), &amount)) {
        return amount.i;
    }
    return 0;
}

bool ioopm_store_cart_remove_item(store_t *store, int cart_id, char *name, int amount) {
    carts_t *cart = find_cart(store, cart_id);
    if (amount < 0 || cart == NULL) return false;
    if (amount == 0) return true;
    if (amount > ioopm_store_cart_item_count(store, cart_id, name)) return false;

    int new_amount = ioopm_store_cart_item_count(store, cart_id, name) - amount;
    if (new_amount > 0) {
        ioopm_hash_table_insert(cart->items, string_elem(name), int_elem(new_amount));
        return true;
    }

    char *stored_key = NULL;
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->items);
    while (!ioopm_hash_table_iterator_at_end(it)) {
        char *key = ioopm_hash_table_iterator_current_key(it).s;
        if (strcmp(key, name) == 0) {
            stored_key = key;
            break;
        }
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);

    elem_t removed;
    ioopm_hash_table_remove(cart->items, string_elem(name), &removed);
    free(stored_key);
    return true;
}

/// @brief removes merch (name) from all carts
static void remove_from_all_carts(store_t *store, char *name) {
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(store->carts_db);
    while(!ioopm_hash_table_iterator_at_end(it)) {
        int cart_id = ioopm_hash_table_iterator_current_key(it).i;
        int amount = ioopm_store_cart_item_count(store, cart_id, name);
        if (amount > 0) {
            ioopm_store_cart_remove_item(store, cart_id, name, amount);
        }
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);
}

int ioopm_store_cart_cost(store_t *store, int cart_id) {
    carts_t *cart = find_cart(store, cart_id);
    if (cart == NULL) return -1;

    int total = 0;
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->items);
    while (!ioopm_hash_table_iterator_at_end(it)) {
        char *name = ioopm_hash_table_iterator_current_key(it).s;
        int amount = ioopm_hash_table_iterator_current_value(it).i;
        total += amount * ioopm_merch_price(ioopm_store_get_merch(store, name));
        ioopm_hash_table_iterator_advance(it);
    }

    ioopm_hash_table_iterator_destroy(it);
    return total;
}

/// @brief Removes a location that has reached quantity 0. Removed first from shelf_db
///        then from merch's location list and then frees the memory
static void remove_empty_location(store_t *store, merch_t *merch, location_t *loc) {
    elem_t ignored;
    ioopm_hash_table_remove(store->shelf_db, string_elem(loc->shelf), &ignored);

    for (size_t i = 0; i < ioopm_list_size(merch->locations); ++i) {
        if (ioopm_list_get(merch->locations, i).p == loc) {
            ioopm_list_remove(merch->locations, i);
            break;
        }
    }
    free(loc->shelf);
    free(loc);
}


/// @brief Decreases amount of merch items from stock . Takes from shelfs in alpabethical order
///        and removed shelves that have become empty
/// @pre the stock is at least as big as amount
static void take_from_stock(store_t *store, merch_t *merch, int amount) {
    int count;
    location_t **array = sorted_locations(store, merch->name, &count);
    int remaining = amount;

    for (int i = 0; i < count && remaining > 0; ++i) {
        location_t *loc = array[i];
        int taken = remaining < loc->quantity ? remaining : loc->quantity;
        loc->quantity -= taken;
        remaining -= taken;
        if (loc->quantity == 0) {
            remove_empty_location(store, merch, loc);
        }
    }
    assert(remaining == 0);
    free(array);
}

bool ioopm_store_checkout(store_t *store, int cart_id) {
    carts_t *cart = find_cart(store, cart_id);
    if (cart == NULL) return false;

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->items);
    while (!ioopm_hash_table_iterator_at_end(it)) {
        char *name = ioopm_hash_table_iterator_current_key(it).s;
        int amount = ioopm_hash_table_iterator_current_value(it).i;
        take_from_stock(store, ioopm_store_get_merch(store, name), amount);
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);
    return ioopm_store_cart_remove(store, cart_id);

}
