#pragma once

#include <stdlib.h>
#include "common.h"

/// @brief  Represents a single link in the liked list
typedef struct link link_t;

/// @brief structure of the linked list nodes (links) holding an elem_t value
struct link {
    elem_t value; // the value stored in the link
    link_t *next; // a pointer to the next link in the list or NULL if end of list
};

/// @brief A linked list with one sentinel node and a tail pointer
struct list {
    link_t *first; //pointer to sentinel node
    link_t *last; // pointer to last link in list, or sentinel is list is empty
    size_t size; // number of element in list, excluding sentinel node, gives size in O(1)
};

/// @brief Allocates and initialises a new link 
/// @param value the value stored in the link
/// @param next pointer to the next link in the list or NULL if end of list
/// @return pointer to the link we just created 
link_t *ioopm_link_create(elem_t value, link_t *next);
