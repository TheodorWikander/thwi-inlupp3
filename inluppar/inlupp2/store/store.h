#pragma once

#include "../generic_utils/utils.h"
#include "../generic_structures/all_generic_structures.h"
#include <stdbool.h>

typedef struct merch merch_t;
typedef struct location location_t;
typedef struct carts carts_t;
typedef struct store store_t;

store_t *ioopm_store_create(void);

void ioopm_store_destroy(store_t *store);

/// @brief get the name of a merch
char *ioopm_merch_name(merch_t *merch);

/// @brief get the description of a merch
char *ioopm_merch_description(merch_t *merch);

/// @brief get the price of a merch
size_t ioopm_merch_price(merch_t *merch);

/// @brief adds new merch to the store with a name (string), describtion (string), and a price (int)
/// A newly added merch is not in stock. Adding new merch with the same name as existing merch will return false
/// @param store the warehouse the merch will be added into
/// @param name the name of the new merch
/// @param desc the description of what the new emrch is
/// @param price the price of the new merch
/// @return true if the action succeded and a new merch has been added to the warehouse
bool ioopm_store_add_merch(store_t *store, char *name, char *desc, int price);

/// @brief checks if there is any merch with the name, "name" in store
/// @param store the store we look for the merch in
/// @param name the name of the merch we are looking for
/// @return true if store has a merch item with name "name", false outerwise
bool ioopm_store_has_merch(store_t *store, char *name);

/// @brief get the merch item with the name "name" if it exists
/// @param store the store we look for the merch in
/// @param name the name of the merch we are looking for
/// @return merch item with name as its name of null if merch is not in store
merch_t *ioopm_store_get_merch(store_t *store, char *name);

/// @brief removes merch item with name "name"
/// @param store the store to look for merch in
/// @param name the name of the merch we are looking for
/// @return true if merch is succesfully removed from store, false otherwise
bool ioopm_store_remove_merch(store_t *store, char *name);


/// @brief editds merch with name "old_name" and updated it with new_name, new_desc and new_price
/// @param store the store merch is in
/// @param old_name the old name to identify the merch to be edited
/// @param new_name the new name to give the merch
/// @param new_desc the new description of the merch
/// @param new_price the new peice of the merch
/// @return true if edit was succesfull, false outerwise
bool ioopm_store_edit_merch(store_t *store, char *old_name, char *new_name, char *new_desc, int new_price);

/// @brief returns the count of merch item in store
/// @param store 
/// @return the ampunt of items in store;
size_t ioopm_store_merch_count(store_t *store);


/// @brief Adds items of a merch to a shelf. Creates the shelf location if needed.
/// @param shelf shelf id: one capital letter A-Z followed by two digits (e.g. "A25")
/// @param name name of an existing merch
/// @param amount number of items to add, must be >= 1
/// @return false (and nothing changed) if the merch does not exist, the shelf name
///         is invalid, amount < 1, or the shelf already holds a different merch
bool ioopm_store_replenish(store_t *store, char *shelf, char *name, int amount);

/// @brief Total stock of a merch, summed over all shelves
/// @return the stock, or 0 if the merch does not exist
int ioopm_store_merch_stock(store_t *store, char *name);


int ioopm_store_merch_shelves(store_t *store, char *name, char ***shelves, int **quantities);