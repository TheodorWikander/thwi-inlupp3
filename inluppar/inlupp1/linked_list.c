#include "linked_list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linked_list_struct.h"

/// @brief Finds the link immediately preceding the given index in the list, O(n)
/// @param list The linked list to iterate through.
/// @param index The target index.
/// @return A pointer to the link before the link at index or the sentinel node if the list if empty
static link_t *find_previous_link(ioopm_list_t *list, size_t index) {
    link_t *previous = list->first; 

    for (size_t i = 0; i < index; ++i) {
        previous = previous->next;
    }
    return previous;
}

link_t *ioopm_link_create(elem_t value, link_t *next) {
    link_t *link = calloc(1, sizeof(link_t));
    link->value = value;
    link->next = next;
    return link;
}

ioopm_list_t *ioopm_list_create(void) {
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
    link_t *sentinel = calloc(1, sizeof(link_t));

    list->first = sentinel;
    list->last = sentinel;
    return list;
}

void ioopm_list_destroy(ioopm_list_t *list) {
    link_t *current = list->first;
    //free all links first
    while (current != NULL) {
        link_t *next = current->next; // spara next innan vi frigör current
        free(current);
        current = next;
    }
    //when all links have been freed, free list. 
    free(list);
}

void ioopm_list_append(ioopm_list_t *list, elem_t value) {

    link_t *new_link = ioopm_link_create(value, NULL);

    list->last->next = new_link;
    list->last = list->last->next;
    list->size += 1;
}

void ioopm_list_prepend(ioopm_list_t *list, elem_t value) {
    //ioopm_list_insert is O(n) because of the for loop in find_previous_link, however since loop is only run one time
    //when index = 0 ioopm_list_insert(list, 0, value) is O(1), therefore ioopm_list_prepend is also O(1) even though it is using ioopm_list_insert. 
    ioopm_list_insert(list, 0, value);

    ////old code before insert was implemented:

        // //create new link containing value and the first real node after the sentinel
        // link_t *new_link = link_create(value, list->first->next);

        // //make sentinel node point to our new link
        // list->first->next = new_link;

        // //check if sentinel was the only node
        // //if it was list.last was also pointing at sentinel and must now be updated
        // if (list->last == list->first) {
        //     list->last = list->first->next;
        // }

        // list->size += 1;
    
}

elem_t ioopm_list_head(ioopm_list_t *list) {
    assert(!ioopm_list_is_empty(list));
    //since list.first is the sentinel node
    return list->first->next->value;
}

elem_t ioopm_list_last(ioopm_list_t *list) {
    assert(!ioopm_list_is_empty(list));
    return list->last->value;
}

void ioopm_list_insert(ioopm_list_t *list, size_t index, elem_t value) {
    assert(index <= ioopm_list_size(list));
    
    link_t *previous = find_previous_link(list, index);
    previous->next = ioopm_link_create(value, previous->next);
    
    if (previous == list->last) {
        list->last = previous->next;
    }
    list->size += 1;
}

elem_t ioopm_list_remove(ioopm_list_t *list, size_t index) {
    assert(index < list->size);

    link_t *previous = find_previous_link(list, index);
    link_t *to_remove = previous->next;
    elem_t value_removed = to_remove->value;

    if (to_remove == list->last) {
        list->last = previous;
    }

    previous->next = to_remove->next;
    free(to_remove);
    list->size = list->size -1;
    return value_removed;
}

elem_t ioopm_list_get(ioopm_list_t *list, size_t index) {
    //assert to crash the program if precondition is not followed
    assert(index < list->size) ;
    link_t *previous = find_previous_link(list, index);
    return previous->next->value;
}

size_t ioopm_list_size(ioopm_list_t *list) {
    return list->size;
}

bool ioopm_list_is_empty(ioopm_list_t *list) {
    if (list->size == 0) {
        return true;
    }
    else {
        return false;
    }
}