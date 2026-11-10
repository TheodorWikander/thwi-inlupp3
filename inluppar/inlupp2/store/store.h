#pragma once

#include "../generic_utils/utils.h"
#include "../generic_structures/all_generic_structures.h"
#include <stdbool.h>

/**
 * @file store.h
 * @brief Bussiness logic of the webstore backend: merchandise, stock on shelves and carts. 
 *        Contains no user interface code. 
 * 
 * Ownership: The store owns everything it holds. All strings passed into the store are copied,
 * so the callers may free their own strings right after a call.
 * Pointers returned by the store belongs to the store and must never be freed by the caller. 
 * 
 * Failiures that depend on user input (invalid merch names, carts etc...) leaves the store unchanged
 * 
 * Violating a @pre will crash the program (assert)
 */

typedef struct merch merch_t;
typedef struct location location_t;
typedef struct carts carts_t;
typedef struct store store_t;

/// @brief Creates a new empty store
/// @return a new store. The fist cart created gets id 1. 
store_t *ioopm_store_create(void);

/// @brief Destroys a store and frees all the merch, shelves and carts in the store
/// @param store the store to destroy
void ioopm_store_destroy(store_t *store);


// ---------- M E R C H A N D I S E ----------


/// @brief get the name of a merch, valid until merch is edited or removed (do not free)
char *ioopm_merch_name(merch_t *merch);

/// @brief get the description of a merch, valid until merch is edited or removed (do not free)
char *ioopm_merch_description(merch_t *merch);

/// @brief get the price of a merch
int ioopm_merch_price(merch_t *merch);

/// @brief adds new merch to the store with a name (string), describtion (string), and a price (int)
/// A newly added merch is not in stock. 
/// @param store the warehouse the merch will be added into
/// @param name the unique name of the new merch
/// @param desc the description of what the new merch is
/// @param price the price of the new merch, must be >= 0
/// @return true if the action succeded, false (store unchanged) if name already exists in store or price is negative
bool ioopm_store_add_merch(store_t *store, char *name, char *desc, int price);

/// @brief checks if there is any merch with the name, "name" in store
/// @param store the store we look for the merch in
/// @param name the name of the merch we are looking for
/// @return true if store has a merch item with name "name", false otherwise
bool ioopm_store_has_merch(store_t *store, char *name);

/// @brief get the merch item with the name "name" if it exists
/// @param store the store we look for the merch in
/// @param name the name of the merch we are looking for
/// @return merch item with name as its name of NULL if merch is not in store, Owned by the store;
///         valid until that merchandise is removed.
merch_t *ioopm_store_get_merch(store_t *store, char *name);

/// @brief removes merch item with name "name", including its stock, shelves (who becomes free for other merch)
///        and all of its items in every cart
/// @param store the store to look for merch in
/// @param name the name of the merch we are looking for
/// @return true if merch is succesfully removed from store, false otherwise (no such merch)
bool ioopm_store_remove_merch(store_t *store, char *name);

/// @brief editds merch with name "old_name" and updated it with new_name, new_desc and new_price
///        Stock and cart contents are kept the same (renamed merch is renamed in carts as well)
/// @param store the store merch is in
/// @param old_name the old name to identify the merch to be edited
/// @param new_name the new name to give the merch
/// @param new_desc the new description of the merch
/// @param new_price the new peice of the merch
/// @return true if edit was succesfull, false (store unchanged) otherwise (i.e. if new_price is negative, 
///         old_name does not exist or new_name is used by another merchandise )
bool ioopm_store_edit_merch(store_t *store, char *old_name, char *new_name, char *new_desc, int new_price);

/// @brief returns the total count of all merch items in store
size_t ioopm_store_merch_count(store_t *store);

/// @brief Name of the index:th merch in alphabetical order
/// @pre 0 <= index < ioopm_store_merch_count(store)
/// @return the name of the merch at index. Owned by the store; valid until that merchandise is
///         edited or removed.
char *ioopm_store_merch_name_at(store_t *store, int index);


// ---------- S T O C K ----------


/// @brief Adds items of a merch to a shelf. Creates the shelf location if needed.
/// @param shelf shelf id: one capital letter A-Z followed by two digits (e.g. "A25")
/// @param name name of an existing merch
/// @param amount number of items to add, must be >= 1
/// @return true if added. false (store unchanged) if the merch does not exist, the shelf name
///         is invalid, amount < 1, or the shelf already holds a different merch
bool ioopm_store_replenish(store_t *store, char *shelf, char *name, int amount);

/// @brief Total stock of a merch, summed over all shelves
/// @return the stock, or 0 if the merch does not exist
int ioopm_store_merch_stock(store_t *store, char *name);

/// @brief Number of shelves that holds a merch item
/// @return the number of shelves, or 0 if the merch does not exist or has no stock
int ioopm_store_shelf_count(store_t *store, char *name);

/// @brief Name of the index:th shelf holding the merch (name) in alphabethical order
/// @pre 0 <= index < ioopm_store_shelf_count(store, name)
/// @return the shelf name. Owned by the store; valid until the shelf is emptied
///         (by checkout) or the merchandise is removed.
char *ioopm_store_shelf_name(store_t *store, char *name, int index);

/// @brief Quantity on the index_th shelf sorted in alphabethical order 
/// @pre 0 <= index < ioopm_store_shelf_count(store, name)
/// @return the qunatity on the index:th shelf. 
int ioopm_store_shelf_quantity(store_t *store, char *name, int index);


// ---------- C A R T S ----------


/// @brief Creates a new empty cart
/// @return the cart id: 1 for the first cart, then 2, 3, ... (id's are never reused!)
int ioopm_store_cart_create(store_t *store);

/// @brief removes a cart (Stock of merches in cart unchanged)
/// @return true if removal succeded. False if the cart does not exist
bool ioopm_store_cart_remove(store_t *store, int cart_id);

/// @brief Checks if cart with cart id exist i store
/// @return true if the cart exists, false otherwise
bool ioopm_store_has_cart(store_t *store, int cart_id);

/// @brief adds an amount of a merch item (name) to a cart (cart_id). All carts together can never hold
///        a bigger amount of a merch than its stock (so every cart can be checked out)
/// @return true if succesfully added, false if amount < 1, invalid item name or invalid cart_id or if the carts would hold more than the stock
bool ioopm_store_cart_add(store_t *store, int cart_id, char *name, int amount);

/// @brief Removes amount items of a merch from a cart. An item that reaches 0
///        disappears from the cart.
/// @param amount number of items to remove: 0 does nothing, otherwise at most
///        the amount the cart holds
/// @return true if removed (or amount is 0). false (store unchanged) if the cart
///         does not exist, amount is negative or larger than what the cart holds
bool ioopm_store_cart_remove_item(store_t *store, int cart_id, char *name, int amount);

/// @return amount of items (name) in cart (cart_id). 0 if the cart does not exist or does not hold the merch item(name)
int ioopm_store_cart_item_count(store_t *store, int cart_id, char *name);

/// @brief sums the price of all items in a cart
/// @return the price of all items in a cart. 0 for an empty cart and (-1) if the cart does not exist
int ioopm_store_cart_cost(store_t *store, int cart_id);

/// @brief buys everything in a cart. Decreases stock(taking form shelves in alphabetical order, empty shelves are removed) and removes the cart
/// @return true if it succeeds, false if the cart does not exist
bool ioopm_store_checkout(store_t *store, int cart_id);
