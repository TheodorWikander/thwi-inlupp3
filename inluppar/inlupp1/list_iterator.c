#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "list_iterator.h"
#include "linked_list_struct.h"
#include "linked_list.h"

struct list_iterator {
    ioopm_list_t *list;
    link_t *previous; //link to sentinel node
};

ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *list) {

    ioopm_list_iterator_t *it = calloc(1, sizeof(ioopm_list_iterator_t));

    it->list = list;
    it->previous = list->first;

    return it;
} 

void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iter) {
        free(iter);
}

bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iter) {
    if (iter->previous->next == NULL) {
        return true;
    }
     else {
        return false;
     }
}

void ioopm_list_iterator_advance(ioopm_list_iterator_t *iter) {
    assert(!ioopm_list_iterator_at_end(iter));
        iter->previous = iter->previous->next;
}

elem_t ioopm_list_iterator_current(ioopm_list_iterator_t *iter) {
    assert(!ioopm_list_iterator_at_end(iter));
    return iter->previous->next->value;
   
}

/// NOTE: REMOVE IS OPTIONAL TO IMPLEMENT
/// @brief Remove the current element from the underlying list
/// @param iter the iterator
/// @return the removed element
elem_t ioopm_list_iterator_remove(ioopm_list_iterator_t *iter) {
    //TODO: Stub
    (void) iter;
    return int_elem(-1);
}

/// @brief Insert a new element into the underlying list making the current element it's next
/// @param iter the iterator
/// @param element the element to be inserted
void ioopm_list_iterator_insert(ioopm_list_iterator_t *iter, elem_t element) {
    link_t *new_link = ioopm_link_create(element, iter->previous->next);
    iter->previous->next = new_link;

    if (iter->previous == iter->list->last) {
        iter->list->last = new_link;
    }

    iter->list->size += 1;
}